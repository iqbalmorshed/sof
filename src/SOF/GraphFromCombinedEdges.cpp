/*
 * GraphFromCombinedEdges.cpp
 *
 *  Created on: Sep 16, 2018
 *      Author: iqbal
 */

#include "GraphFromCombinedEdges.h"
#include "Timer.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

namespace sof {

GraphFromCombinedEdges::GraphFromCombinedEdges(const ReadsInfo& readsInfo, const readLen_t minOverlap) :
												m_readsInfo(readsInfo),
												m_numReads(readsInfo.get_numReads()),
												m_minOverlap(minOverlap){
	// TODO Auto-generated constructor stub
	Timer t("Pruning done in Combined edges:");

	Edges edgesBE, edgesEE;
	get_edges_from_file("temp_edge_file.be", FORWARD, edgesBE);
	get_edges_from_file("temp_edge_file.ee", FORWARD, edgesEE);

	construct_edges(edgesBE, FORWARD, edgesEE, FORWARD, edgesEE, "temp_edge_file.ee");
	Edges().swap(edgesEE);

	//construct (B,B) edges
	Edges edgesBER;
	get_reverse_edges(edgesBE, edgesBER);
	Edges().swap(edgesBE);

	Edges edgesBB;
	get_edges_from_file("temp_edge_file.bb", FORWARD, edgesBB);

	construct_edges(edgesBER, REVERSE, edgesBB, FORWARD, edgesBB, "temp_edge_file.bb");
	Edges().swap(edgesBER);

	//construct (B,E) edges
	Edges edgesEER;
	get_edges_from_file("temp_edge_file.ee", REVERSE, edgesEER);

	construct_edges(edgesEER, REVERSE, edgesBB, FORWARD, "temp_edge_file.be", "temp_edge_file.be.new");
	Edges().swap(edgesEER);

	Edges edgesBBR;
	get_reverse_edges(edgesBB, edgesBBR);
	Edges().swap(edgesBB);

	get_edges_from_file("temp_edge_file.ee", FORWARD, edgesEE);
	construct_edges(edgesEE, FORWARD, edgesBBR, REVERSE, "temp_edge_file.be.new", "temp_edge_file.be");
	Edges().swap(edgesEE);
	Edges().swap(edgesBBR);
}



void GraphFromCombinedEdges::get_edges_from_file(const std::string& fileName,
										const EdgeDirection edgeDirection,
										Edges& edges) {

	edges =  Edges(m_numReads);

	std::ifstream inputFile(fileName);

	while (inputFile) {
		// read stuff from the file into a string and print it
		std::string strInput;
		getline(inputFile, strInput);

		std::stringstream ss;
		numReads_t sourceVertex, destinationVertex;
		readLen_t overlapLength;

		ss << strInput;
		ss >> sourceVertex;

		while (ss >> destinationVertex >> overlapLength) {

			DestinationVertexInfo destinationVertexInfo;

			destinationVertexInfo.overlapLength = overlapLength;
			if(edgeDirection == FORWARD){
				destinationVertexInfo.destinationVertex = destinationVertex;
				edges[sourceVertex].push_back(destinationVertexInfo);
			}
			else{ //REVERSE
				destinationVertexInfo.destinationVertex = sourceVertex;
				edges[destinationVertex].push_back(destinationVertexInfo);
			}
		}
	}



}

void GraphFromCombinedEdges::construct_edges(const Edges& xyEdges,
												const EdgeDirection xyEdgeDirection,
												const Edges& yzEdges,
												const EdgeDirection yzEdgeDirection,
												const Edges& xzEdges,
												const std::string& tempFileName) {

	bool isXFullListSearch = (xyEdgeDirection == REVERSE)? true: false;
	bool isYFullListSearch = (yzEdgeDirection == REVERSE)? true: false;


	m_transitive = std::vector<numReads_t>(m_numReads, -1);

	std::ofstream tempFileWriter(tempFileName);

	for(numReads_t virtualID = 0; virtualID < m_numReads; virtualID++){

		if (m_readsInfo.get_isVertex(virtualID) && xzEdges[virtualID].size()) {

			readLen_t xzMinOverlap = xzEdges[virtualID].rbegin()->overlapLength;
			mark_transitive(virtualID, xyEdges, yzEdges, isXFullListSearch, isYFullListSearch, xzMinOverlap);
			//write irriducible edges in file
			write_in_temp_file(virtualID, xzEdges, tempFileWriter);

		}
	}

}

void GraphFromCombinedEdges::write_in_temp_file(const numReads_t virtualID,
						const Edges& xzEdges,
						std::ofstream& tempFileWriter){

	bool isSourcePrinted = false;

	for(auto xIterator = xzEdges[virtualID].begin();
			xIterator != xzEdges[virtualID].end(); xIterator++){

		numReads_t destinationVertex = xIterator->destinationVertex;
		readLen_t overlapLength = xIterator->overlapLength;

		if( m_transitive[destinationVertex] != virtualID){

			if(!isSourcePrinted){
				tempFileWriter << virtualID << " ";
				isSourcePrinted = true;
			}
			tempFileWriter << destinationVertex << " "<< int(overlapLength) << " ";
		}

	}
	if(isSourcePrinted)
		tempFileWriter << '\n';

}

void GraphFromCombinedEdges::get_reverse_edges(const Edges& sourceEdges, Edges& reverseEdges)
{
	reverseEdges = Edges(m_numReads);
	DestinationVertexInfo destinationVertexInfo;

	for(numReads_t virtualID =0 ; virtualID < m_numReads; virtualID++){

		if(m_readsInfo.get_isVertex(virtualID) && sourceEdges[virtualID].size()){

			for(auto it = sourceEdges[virtualID].begin(); it != sourceEdges[virtualID].end() ; it++ ){
				destinationVertexInfo.destinationVertex = virtualID;
				destinationVertexInfo.overlapLength = it->overlapLength;
				reverseEdges[it->destinationVertex].push_back(destinationVertexInfo);
			}
		}
	}
}

void GraphFromCombinedEdges::construct_edges(const Edges& xyEdges,
		const EdgeDirection xyEdgeDirection, const Edges& yzEdges,
		const EdgeDirection yzEdgeDirection, const std::string& xzEdgesFileName,
		const std::string& tempFileName) {

	bool isXFullListSearch = (xyEdgeDirection == REVERSE)? true: false;
	bool isYFullListSearch = (yzEdgeDirection == REVERSE)? true: false;

	m_transitive = std::vector<numReads_t>(m_numReads, -1);

	std::ifstream inputFile(xzEdgesFileName);
	std::ofstream tempFileWriter(tempFileName);

	while (inputFile) {
		// read stuff from the file into a string and print it
		std::string strInput;
		getline(inputFile, strInput);

		std::stringstream ss;
		numReads_t sourceVertex, destinationVertex;
		readLen_t overlapLength;

		ss << strInput;
		ss >> sourceVertex;

		mark_transitive(sourceVertex, xyEdges, yzEdges, isXFullListSearch, isYFullListSearch, m_minOverlap);

		bool isSourceVertexPrinted = false;
		while (ss >> destinationVertex >> overlapLength) {

			if(m_transitive[destinationVertex] != sourceVertex){

				if(!isSourceVertexPrinted){
					tempFileWriter << sourceVertex <<" ";
					isSourceVertexPrinted = true;
				}

				tempFileWriter << destinationVertex <<" "<<overlapLength<<" ";
			}
		}

		if(isSourceVertexPrinted)
			tempFileWriter<<'\n';

	}

	std::remove(xzEdgesFileName.c_str());

}

void GraphFromCombinedEdges::mark_transitive(const numReads_t virtualID,
						const Edges& xyEdges,
						const Edges& yzEdges,
						const bool isXFullListSearch,
						const bool isYFullListSearch,
						const readLen_t xzMinOverlap){

	for(auto xIterator = xyEdges[virtualID].begin();
			xIterator != xyEdges[virtualID].end(); xIterator++){

		readLen_t xyOverlap = xIterator->overlapLength;

		if(xyOverlap > xzMinOverlap) {

			numReads_t yID = xIterator->destinationVertex;
			readLen_t yzMinOverlap = m_readsInfo.get_readLen(yID) - xyOverlap + xzMinOverlap;

			for(auto yIterator = yzEdges[yID].begin();
					yIterator != yzEdges[yID].end(); yIterator++) {

				readLen_t yzOverlap = yIterator->overlapLength;
				if(yzOverlap >= yzMinOverlap) {
					m_transitive[yIterator->destinationVertex]= virtualID;
				}
				else if(!isYFullListSearch)
					break;
			}
		}
		else if(!isXFullListSearch)
			break;
	}


}

} /* namespace sof */
