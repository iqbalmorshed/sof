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
	ReadOperations(	const ChunkInfo& chunkInfo,
					const OverlapContainer& overlapContainer,
					const LexicographicIndex& lexicoIndex,
					ReadsInfo& readsInfo,
					const std::string& readsFileName,
					const readLen_t minOverlap);
	virtual ~ReadOperations();

	bool get_read(CurrentRead& currentRead);
	void filter_edges(CurrentRead& currentRead);
	void write_edges(CurrentRead& currrentRead);

protected:
	const ChunkInfo m_chunkInfo;
	const OverlapContainer& m_overlapContainer;
	const LexicographicIndex& m_lexicoIndex;
	const readLen_t m_minOverlap;
	ReadsInfo& m_readsInfo;
	const std::string& m_readsFileName;
	std::ifstream m_tempEdgeReader;
	std::ofstream m_tempEdgeWriter;
	char m_buffer[BUFSIZ];
	int m_charsCollectedInBuffer = 0;
	int m_totalCharsInBuffer = 0;
	int m_itemNoInLine = 0;
	size_t m_item = 0;

	bool m_isValidRead = 1;

private:
	//CurrentRead& m_currentRead;
	void process_item(CurrentRead& currentRead,
			OverlapInfo& overlapInfo);
	void filter_using_read(	CurrentRead& currentRead,
							numReads_t virtualID,
							readLen_t positionCurrentRead);
	inline bool isBothReverseComplement(bool isSourceRevComp, numReads_t virtualID){
		return isSourceRevComp && virtualID%2;
	}
	inline bool isWithinRange(numReads_t virtualID){
		return virtualID >= m_chunkInfo.start && virtualID <= m_chunkInfo.end;
	}
	void write_partially_filtered_edges(CurrentRead& currentRead);
	void write_completely_filtered_edges(CurrentRead& currentRead);


};

} /* namespace sof */

#endif /* SRC_SOF_READOPERATIONS_H_ */
