/*
 * StringGraph.cpp
 *
 *  Created on: Jun 17, 2018
 *      Author: iqbal
 */

#include "StringGraph.h"

#include <iostream>
#include <string>
#include <chrono>

#include "BWTBySampling.h"
#include "CurrentRead.h"
#include "FileWriter.h"
#include "LexicographicIndex.h"
#include "OverlapContainer.h"
#include "ReadOperations.h"
#include "ReadsInfo.h"
#include "RepeatRemoval.h"
#include "Timer.h"

namespace sc = std::chrono;
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
		std::cout<<"BWT: number of reads: "<<pBWT->get_numReads()<<'\n';
		std::cout<<"BWT: number of bases: "<<pBWT->get_bwtLength()<<'\n';

		pBWT->printInfo();

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

//		overlapContainer.readFromFile();
//		std::cout<<"overlapContainer loaded from file\n";

		construct_edges_using_partial_container(overlapContainer,
				readsInfo, lexicoIndex);

		// Construct BE Edges
//		construct_edges(nullptr, overlapContainer, BE_EDGE, readsInfo,
//						lexicoIndex, m_inputData.readsFileName,
//						m_inputData.minOverlap);



	}

	RepeatRemoval repeatRemoval(readsInfo.get_numReads());

	FileWriter fileWriter;
	fileWriter.write_asqg_file(m_inputData, readsInfo);

	std::cout<<"string graph constructed.\n";

	return true;
}

void StringGraph::construct_edges(	const ChunkInfo& chunkInfo,
									const OverlapContainer& overlapContainer,
									ReadsInfo& readsInfo,
									const LexicographicIndex& lexicoIndex,
									const std::string& readsFileName,
									const readLen_t minOverlap) {

	sc::microseconds getReadTime(0),filterTime(0), writeTime(0);
	Timer t("Edge construction time:");
	ReadOperations readOp(	chunkInfo, overlapContainer, lexicoIndex,
							readsInfo, readsFileName, minOverlap);
	std::cout<<"readOp successfully created"<<std::endl;

	auto numReads = readsInfo.get_numReads();

	CurrentRead currentRead;

	auto t1 = sc::high_resolution_clock::now();
	while (readOp.get_read(currentRead)) {
		auto t2 = sc::high_resolution_clock::now();
		getReadTime+= sc::duration_cast<sc::microseconds>(t2 - t1);
		//currentRead.print_intervals();
		//std::cout<<"collected current read"<<std::endl;
		readOp.filter_edges(currentRead);
		auto t3 = sc::high_resolution_clock::now();
		filterTime+= sc::duration_cast<sc::microseconds>(t3 - t2);
		//std::cout<<"filtered current read"<<std::endl;
		readOp.write_edges(currentRead);
		auto t4 = sc::high_resolution_clock::now();
		writeTime+= std::chrono::duration_cast<sc::microseconds>(t4 - t3);
		//currentRead.print_intervals();
		t1 = sc::high_resolution_clock::now();
	}
	sc::microseconds total_time_ms = getReadTime+filterTime+writeTime;
	sc::seconds total_time_s = sc::duration_cast<std::chrono::seconds> (total_time_ms);
	std::cout<<"Total read time: "<<getReadTime.count()<<" Total filter time: "
			<<filterTime.count()<<" Total write :"<<writeTime.count()<<'\n';
	std::cout<<"Total chunk time (ms): "<<total_time_ms.count()<<'\n';
	std::cout<<"Total chunk time (sec): "<<total_time_s.count()<<'\n';

	std::cout <<"Chunk: "<< chunkInfo.ID << " type edge construction Completed" << std::endl;



}

void StringGraph::construct_edges_using_partial_container(
		OverlapContainer& overlapContainer,
		ReadsInfo& readsInfo,
		const LexicographicIndex& lexicoIndex) {
//	int chunkSize = readsInfo.get_numReads() / numParts + 1;

	ChunkInfo chunkInfo;
	while(chunkInfo.end != readsInfo.get_numReads()-1){

		//construct_partial_container(overlapContainer, chunkInfo);
		//std::cout<<"inside construct_edges_using_partial_container:\n";
		chunkInfo = overlapContainer.readPartiallyFromFile(chunkInfo);
		//std::cout<<"container created\n";
		construct_edges(chunkInfo, overlapContainer, readsInfo,
						lexicoIndex, m_inputData.readsFileName,
						m_inputData.minOverlap);

		std::cout<<"chunkInfo.end: "<<chunkInfo.end <<"num of Reads: "<<readsInfo.get_numReads()<<'\n';
		std::cout<<"chunkID: "<<chunkInfo.ID<<"finished\n\n";
	}


}

} /* namespace sof */
