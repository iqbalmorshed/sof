/*
 * StringGraph.h
 *
 *  Created on: Jun 17, 2018
 *      Author: iqbal
 */

#ifndef SRC_SOF_STRINGGRAPH_H_
#define SRC_SOF_STRINGGRAPH_H_

#include <iostream>
#include "BWT.h"
#include "LexicographicIndex.h"
#include "OverlapContainer.h"
#include "SOFCommon.h"
#include "ReadsInfo.h"

namespace sof {

enum BWTType{
	BWT_BY_WAVELET,
	BWT_BY_SAMPLING,
};
struct InputData{
public:
	std::string readsFileName;
	std::string bwtFileName;
	//std::string revBwtFileName;
	std::string lexIndexFileName;
	//std::string revLexIndexFileName;
	readLen_t minOverlap;

	//BWTType bwtType = BWT_BY_WAVELET;
	BWTType bwtType = BWT_BY_SAMPLING;

	double errorRate = 0.0f;
	bool bIrreducibleOnly = true;

};


class StringGraph {
public:
	StringGraph(InputData inputData);

	isDone_t construct();

private:
	void construct_edges(	const ChunkInfo& chunkInfo,
							const OverlapContainer& overlapContainer,
							ReadsInfo& readsInfo,
							const LexicographicIndex& lexicoIndex,
							const std::string& readsFileName,
							const readLen_t minOverlap);
	void construct_edges_using_partial_container(	OverlapContainer& overlapContainer,
													ReadsInfo& readsInfo,
													const LexicographicIndex& lexicoIndex);

	const InputData m_inputData;

};

} /* namespace sof */

#endif /* SRC_SOF_STRINGGRAPH_H_ */
