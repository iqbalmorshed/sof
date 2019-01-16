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
#include "CurrentRead.h"
#include "FileWriter.h"
#include "LexicographicIndex.h"
#include "OperationOnBE.h"
#include "OverlapContainer.h"
#include "ReadOperations.h"
#include "ReadsInfo.h"
#include "RepeatRemoval.h"
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
		pBWT = new BWTBySampling(m_inputData.bwtFileName);

		LexicographicIndex lexicoIndex(m_inputData.lexIndexFileName);

		readsInfo.set_containers_size(pBWT->get_numReads());

		std::cout<<"BWT and LexicoIndex construction completed.\n";
		OverlapContainer overlapContainer(m_inputData.readsFileName, pBWT,
											lexicoIndex, readsInfo,
											m_inputData.minOverlap, true);
		overlapContainer.writeToFile();
		std::cout<<"overlapContainer writing completed.\n";
		delete pBWT;
		std::cout<<"BWT is deleted.\n";

		overlapContainer.readFromFile();
		std::cout<<"overlapContainer loaded from file\n";

		// Construct BE Edges
		construct_edges(nullptr, overlapContainer, BE_EDGE, readsInfo,
						lexicoIndex, m_inputData.readsFileName,
						m_inputData.minOverlap);



	}

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


	readOp = new OperationOnBE(nullptr, overlapContainer, lexicoIndex,
									minOverlap, readsInfo);
	std::cout<<"readOp successfully created"<<std::endl;

	auto numReads = readsInfo.get_numReads();
	for (numReads_t virtualID = 0; virtualID < numReads; virtualID++) {

		//std::cout<<"virtual id processed"<<virtualID<<std::endl;
		if (readsInfo.get_isValid(virtualID)) {
			CurrentRead currentRead = readOp->get_read(virtualID);
			//currentRead.print_intervals();
			///std::cout<<"collected current read"<<std::endl;
			readOp->filter_edges(currentRead);
			//std::cout<<"filtered current read"<<std::endl;
			readOp->write_edges(currentRead);
			//currentRead.print_intervals();

		}

	}

	delete readOp;
	std::cout << edgeType << " type edge construction Completed" << std::endl;



}

} /* namespace sof */
