/*
 * StringGraph.cpp
 *
 *  Created on: Jun 17, 2018
 *      Author: iqbal
 */

#include "StringGraph.h"

#include <iostream>
#include <string>

#include "BWTBySampling.h"
//#include "BWTByWavelet.h"
//#include "CombinedGraph.h"
#include "CurrentRead.h"
#include "GraphFromCombinedEdges.h"
#include "FileWriter.h"
#include "LexicographicIndex.h"
#include "OperationOnBE.h"
#include "OperationOnBBandEE.h"
#include "OverlapContainer.h"
#include "ReadOperations.h"
#include "RepeatRemoval.h"
#include "ReadsInfo.h"
#include "Timer.h"

namespace sof {

StringGraph::StringGraph(InputData inputData)
		: m_inputData(inputData) {

}

isDone_t StringGraph::construct() {

	ReadsInfo readsInfo;

	{
		//construct BWT in your desired way
		BWT *pBWT;
//		if(m_inputData.bwtType == BWT_BY_WAVELET)
//			pBWT = new BWTByWavelet(m_inputData.bwtFileName);
//		else
			pBWT = new BWTBySampling(m_inputData.bwtFileName);

		LexicographicIndex lexicoIndex(m_inputData.lexIndexFileName);

		readsInfo.set_containers_size(pBWT->get_numReads());

		std::cout<<"BWT and LexicoIndex construction completed.\n";
		OverlapContainer overlapContainer(FORWARD_OVERLAPS, m_inputData.readsFileName, pBWT,
											lexicoIndex, readsInfo,
											m_inputData.minOverlap, true);

		std::cout<<"overlapContainer Construction completed.\n";

		// Construct BE Edges
		construct_edges(nullptr, overlapContainer, BE_EDGE, readsInfo,
						lexicoIndex, m_inputData.readsFileName,
						m_inputData.minOverlap);

		construct_edges(pBWT, overlapContainer, BB_EDGE, readsInfo, lexicoIndex,
						m_inputData.readsFileName, m_inputData.minOverlap);

		delete pBWT;
	}

	{
		BWT *pRBWT = new BWTBySampling(m_inputData.revBwtFileName);
		LexicographicIndex revLexicoIndex(m_inputData.revLexIndexFileName);

		//revLexicoIndex.print();
		//ReadsInfo readsInfo;
		OverlapContainer revOverlapContainer(REVERSE_OVERLAPS, m_inputData.readsFileName, pRBWT,
												revLexicoIndex, readsInfo,
												m_inputData.minOverlap, false);

		construct_edges(pRBWT, revOverlapContainer, EE_EDGE, readsInfo,
						revLexicoIndex, m_inputData.readsFileName,
						m_inputData.minOverlap);
	}

	//CombinedGraph combinedGraph(m_inputData, readsInfo);
	//combinedGraph.prune();
	//RepeatRemoval repeatRemoval(readsInfo.get_numReads());

	GraphFromCombinedEdges graphFromCombinedEdges(readsInfo, m_inputData.minOverlap);

	RepeatRemoval repeatRemoval(readsInfo.get_numReads());

	FileWriter fileWriter;
	fileWriter.write_asqg_file(m_inputData, readsInfo);

	std::cout<<"string graph constructed.\n";

	return true;
}

void StringGraph::construct_edges(	const BWT* pBWT,
									const OverlapContainer& overlapContainer,
									EdgeType edgeType,
									ReadsInfo& readsInfo,
									const LexicographicIndex& lexicoIndex,
									const std::string& readsFileName,
									const readLen_t minOverlap) {

	Timer t(std::to_string(edgeType)+"type edge construction time:");
	ReadOperations *readOp;


	if (edgeType == BE_EDGE)
		readOp = new OperationOnBE(nullptr, overlapContainer, lexicoIndex,
									minOverlap, readsInfo);
	else
		readOp = new OperationOnBBandEE(pBWT, overlapContainer, lexicoIndex,
										minOverlap, readsInfo, readsFileName,
										edgeType);

	//std::cout<<"readOp successfully created"<<std::endl;

	auto numReads = readsInfo.get_numReads();
	for (numReads_t virtualID = 0; virtualID < numReads; virtualID++) {

		if (readsInfo.get_isValid(virtualID)) {
			CurrentRead currentRead = readOp->get_read(virtualID);
			//std::cout<<"current Read created"<<std::endl;
			//currentRead.print_intervals();
			readOp->filter_edges(currentRead);
			//if(virtualID==12060320)std::cout<<"current Read filtered"<<std::endl;
			readOp->write_edges(currentRead);
			//currentRead.print_intervals();
			//if(virtualID==12060320)std::cout<<"current read written"<<std::endl;
		}

//		if(!(virtualID%20000)){
//			std::cout<<"number of reads processed : "<<virtualID<<'\n';
//		}
//			if(virtualID > 12060000)
//				std::cout<<virtualID<<'\n';

	}

	delete readOp;
	std::cout << edgeType << " type edge construction Completed" << std::endl;



}

} /* namespace sof */
