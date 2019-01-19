/*
 * ReadOperations.h
 *
 *  Created on: Jun 18, 2018
 *      Author: iqbal
 */

#ifndef SRC_SOF_READOPERATIONS_H_
#define SRC_SOF_READOPERATIONS_H_

#include <fstream>

#include "BWT.h"
#include "CurrentRead.h"
#include "LexicographicIndex.h"
#include "OverlapContainer.h"
#include "ReadsInfo.h"

namespace sof {

class ReadOperations {

public:
	ReadOperations(	const BWT *pBWT,
					const OverlapContainer& overlapContainer,
					const LexicographicIndex& lexicoIndex,
					const readLen_t minOverlap,
					ReadsInfo& readsInfo);
	virtual ~ReadOperations();

	CurrentRead get_read(	numReads_t virtualReadID);
	void filter_edges(CurrentRead& currentRead);
	void write_edges(CurrentRead& currrentRead);

protected:
	const BWT* m_pBWT;
	const OverlapContainer& m_overlapContainer;
	const LexicographicIndex& m_lexicoIndex;
	const readLen_t m_minOverlap;
	ReadsInfo& m_readsInfo;
	std::ofstream m_tempEdgeWriter;

private:
	void filter_using_read(	CurrentRead& currentRead,
							numReads_t virtualID,
							readLen_t positionCurrentRead);
	inline bool isBothReverseComplement(bool isSourceRevComp, numReads_t virtualID){
		return isSourceRevComp && virtualID%2;
	}

};

} /* namespace sof */

#endif /* SRC_SOF_READOPERATIONS_H_ */
