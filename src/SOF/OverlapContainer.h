/*
 * OverlapContainer.h
 *
 *  Created on: Jun 18, 2018
 *      Author: iqbal
 */

#ifndef SRC_SOF_OVERLAPCONTAINER_H_
#define SRC_SOF_OVERLAPCONTAINER_H_

#include <iostream>
#include <vector>

#include "BWT.h"
#include "LexicographicIndex.h"
#include "ReadsInfo.h"
#include "SeqReader.h"
#include "SOFCommon.h"

namespace sof {

struct OverlapInfo {
	TerminalInterval terminalInterval;
	readLen_t readIndex;
};
using OverlapInfoVector = std::vector<OverlapInfo>;

struct Read {

	numReads_t virtualID;
	std::string realID ="";
	std::string sequence;
	readLen_t seqLength;
};

enum ContainerType{
	FORWARD_OVERLAPS,
	REVERSE_OVERLAPS,
};

class OverlapContainer {
public:
	OverlapContainer(	const ContainerType containerType,
						const std::string& readsFileName,
						const BWT* pBWT,
						const LexicographicIndex& lexicoIndex,
						ReadsInfo& readsInfo,
						const readLen_t minOverlap,
						const bool bSetReadsInfo);

	const std::vector<OverlapInfo>& operator[](const numReads_t index) const;
	void print();
	numReads_t get_size() const;

private:

	Read get_read(const SeqRecord& record, const numReads_t virtualID );
	void update_readsInfo(const Read& read);
	void update_container(const Read& read);
	void make_repeats_invalid(numReads_t virtualID, char firstBase, BWTInterval& bwtInterval);

	const ContainerType m_containerType;
	const std::string& m_readsFileName;
	const BWT* m_pBWT;
	const LexicographicIndex& m_lexicoIndex;
	ReadsInfo& m_readsInfo;
	const readLen_t m_minOverlap;
	const bool m_bSetReadsInfo;

	const bool m_bIsRepeatReadPresent = DEFAULT_REPEAT_PRESENCE;
	const bool m_bIsProperSubstringPresent = DEFAULT_SUBSTRING_PRESENCE;

	std::vector<std::vector<OverlapInfo> > m_container;

};

} /* namespace sof */

#endif /* SRC_SOF_OVERLAPCONTAINER_H_ */
