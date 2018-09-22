/*
 * CombinedGraph.cpp
 *
 *  Created on: Sep 6, 2018
 *      Author: iqbal
 */

#include "CombinedGraph.h"

#include <string>
#include <cstdio>

#include "SOFCommon.h"
#include "FileWriter.h"
#include "Timer.h"

namespace sof {

CombinedGraph::CombinedGraph(const InputData& inputData,
		const ReadsInfo& readsInfo) :
			m_inputData(inputData),
			m_readsInfo(readsInfo),
			m_numReads(readsInfo.get_numReads()){

	m_adjList = std::vector< EdgeInformation >(m_numReads, EdgeInformation(3));
	m_transitive = std::vector<numReads_t>(m_numReads,-1);
	//m_asqgWriter = std::ofstream(m_inputData.readsFileName + ".asqg");
	//m_realIDVector = std::vector<std::string>(m_numReads);
	//m_edgeStore = EdgeStore(m_numReads);

	// TODO Auto-generated constructor stub
	collect_edges("temp_edge_file.be", BE_EDGE);
	collect_edges("temp_edge_file.bb", BB_EDGE);
	collect_edges("temp_edge_file.ee", EE_EDGE);

	//create_asqg_header_and_vertices();

}

//void CombinedGraph::prune() {
//
//	prune();
//}

void CombinedGraph::prune() {

	Timer t("Combined graph pruning time:");

	for(numReads_t virtualID = 0; virtualID < m_numReads ; virtualID++) {
		if(m_readsInfo.get_isVertex(virtualID) ) {

			if(m_adjList[virtualID][BE_EDGE_FD].size() > 0){

				readLen_t xzMinOverlap = m_adjList[virtualID][BE_EDGE_FD].rbegin()->overlapLength;
				find_transitive_edges(virtualID, EE_EDGE_RV, BB_EDGE_FD, xzMinOverlap);
				find_transitive_edges(virtualID, EE_EDGE_FD, BB_EDGE_RV, xzMinOverlap);
				write_irriducible_edges(virtualID, BE_EDGE);
			}

			if(m_adjList[virtualID][BB_EDGE_FD].size() > 0){

				readLen_t xzMinOverlap = m_adjList[virtualID][BB_EDGE_FD].rbegin()->overlapLength;
				find_transitive_edges(virtualID, BE_EDGE_RV, BB_EDGE_FD, xzMinOverlap);
				write_irriducible_edges(virtualID, BB_EDGE);

			}

			if(m_adjList[virtualID][EE_EDGE_FD].size() > 0){

				readLen_t xzMinOverlap = m_adjList[virtualID][EE_EDGE_FD].rbegin()->overlapLength;
				find_transitive_edges(virtualID, BE_EDGE_FD, EE_EDGE_FD, xzMinOverlap);
				write_irriducible_edges(virtualID, EE_EDGE);

			}
		}
	}
}

void CombinedGraph::find_transitive_edges(const numReads_t virtualID,
										const CombinedEdgeType xyEdgeType,
										const CombinedEdgeType yzEdgeType,
										const readLen_t xzMinOverlap) {

	bool isXFullListSearch = false, isYFullListSearch = false;
	if (m_adjList[virtualID][xyEdgeType].size() > 0) {

		if (xyEdgeType == EE_EDGE_RV || xyEdgeType == BE_EDGE_RV)
			isXFullListSearch = true;

		if(yzEdgeType == BB_EDGE_RV)
			isYFullListSearch = true;

		for(auto xIterator = m_adjList[virtualID][xyEdgeType].begin();
				xIterator != m_adjList[virtualID][xyEdgeType].end(); xIterator++){

			readLen_t xyOverlap = xIterator->overlapLength;

			if(xyOverlap > xzMinOverlap) {

				numReads_t yID = xIterator->destinationVertex;
				readLen_t yzMinOverlap = m_readsInfo.get_readLen(yID) - xyOverlap + xzMinOverlap;

				for(auto yIterator = m_adjList[yID][yzEdgeType].begin();
						yIterator != m_adjList[yID][yzEdgeType].end(); yIterator++) {

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

}

void CombinedGraph::collect_edges(const std::string& tempFileName,const EdgeType edgeType) {

	std::ifstream inputFile(tempFileName);

	while (inputFile) {
		// read stuff from the file into a string and print it
		std::string strInput;
		getline(inputFile, strInput);

		std::stringstream ss;
		numReads_t sourceVertex;
		numReads_t destinationVertex;
		readLen_t sourceIndex;

		//strInput.erase(strInput.find_last_not_of(" \t\n\r\f\v") + 1);

		ss << strInput;
		ss >> sourceVertex;

		while (ss >> destinationVertex >> sourceIndex) {

			DestinationVertexInfo destinationVertexInfo;
			destinationVertexInfo.destinationVertex = destinationVertex;
			destinationVertexInfo.overlapLength = m_readsInfo.get_readLen(sourceVertex) - sourceIndex;

			if(edgeType == BE_EDGE){
				//collect forward edge
				m_adjList[sourceVertex][BE_EDGE_FD].push_back(destinationVertexInfo);

				//collect backward edge
//				destinationVertexInfo.destinationVertex = sourceVertex;
//				m_adjList[destinationVertex][BE_EDGE_RV].push_back(destinationVertexInfo);

			}
			else if(edgeType == BB_EDGE){
				m_adjList[sourceVertex][BB_EDGE_FD].push_back(destinationVertexInfo);

				//collect backward edge
//				destinationVertexInfo.destinationVertex = sourceVertex;
//				m_adjList[destinationVertex][BB_EDGE_RV].push_back(destinationVertexInfo);
			}
			else{ //EE edge
				//collect forward edge
				m_adjList[sourceVertex][EE_EDGE_FD].push_back(destinationVertexInfo);

				//collect backward edge
//				destinationVertexInfo.destinationVertex = sourceVertex;
//				m_adjList[destinationVertex][EE_EDGE_RV].push_back(destinationVertexInfo);

			}

		}
	}

	//std::remove(tempFileName.c_str());
}

void CombinedGraph::write_irriducible_edges(numReads_t virtualID, EdgeType edgeType) {


	numReads_t sourceVertex = virtualID;
	for(auto xIterator = m_adjList[virtualID][edgeType].begin();
			xIterator != m_adjList[virtualID][edgeType].end(); xIterator++){

		numReads_t destinationVertex = xIterator->destinationVertex;
		readLen_t sourceIndex = m_readsInfo.get_readLen(sourceVertex) - xIterator->overlapLength;

		if(m_transitive[destinationVertex] != sourceVertex){

			if(edgeType == BE_EDGE){
				write_edges_in_asqg(edgeType, sourceVertex, destinationVertex, sourceIndex);
			}
			//Since we are writing nodes by the increasing order of sourceVertex,
			//when sourceVertex < destinationVertex we are sure that this edge is
			//not present in m_edgeStore, hence no is_present() check is needed.
			//so when sourceVertex > destinationVertex
			// then there is possibility that edge (destinationVertex, sourceVertex)
			//is already present hence is_present() check is made. This is represented
			//in this one liner.
			else if(sourceVertex < destinationVertex ||
					!m_edgeStore.is_present(edgeType, destinationVertex, sourceVertex , sourceIndex)){
				write_edges_in_asqg(edgeType, sourceVertex, destinationVertex, sourceIndex);
				m_edgeStore.set_edge(edgeType, sourceVertex, destinationVertex, sourceIndex);
			}
		}
	}

}

void CombinedGraph::create_asqg_header_and_vertices() {

	FileWriter fileWriter;

	fileWriter.write_header(m_inputData, m_asqgWriter);
	fileWriter.write_vertexInfo(m_inputData.readsFileName, m_readsInfo, m_realIDVector,
						m_asqgWriter);

}

void CombinedGraph::write_edges_in_asqg(const EdgeType edgeType,
										const numReads_t sourceVertex,
										const numReads_t destinationVertex,
										const readLen_t sourceIndex) {

	m_asqgWriter <<"ED\t"<< m_realIDVector[sourceVertex] << " "
			<< m_realIDVector[destinationVertex] << " ";

	if (edgeType == BE_EDGE) {

		int sourceStartIndex = sourceIndex;
		int sourceLength = m_readsInfo.get_readLen(sourceVertex);
		int sourceEndIndex = sourceLength -1;

		int destinationStartIndex = 0;
		int destinationEndIndex = sourceEndIndex -sourceStartIndex;
		int destinationLength = m_readsInfo.get_readLen(destinationVertex);

		m_asqgWriter << sourceStartIndex << " "
				<< sourceEndIndex << " "
				<< sourceLength << " "
				<< destinationStartIndex<< " "
				<< destinationEndIndex<< " "
				<< destinationLength<< " "
				<< 0 << " " << 0 << '\n';
	}
	else if(edgeType == BB_EDGE){

		int sourceStartIndex = 0;
		int sourceLength = m_readsInfo.get_readLen(sourceVertex);
		int sourceEndIndex = sourceLength - 1 - sourceIndex;

		int destinationStartIndex = 0;
		int destinationEndIndex = sourceEndIndex;
		int destinationLength = m_readsInfo.get_readLen(destinationVertex);

		m_asqgWriter << sourceStartIndex << " "
				<< sourceEndIndex << " "
				<< sourceLength << " "
				<< destinationStartIndex<< " "
				<< destinationEndIndex<< " "
				<< destinationLength<< " "
				<< 1 << " " << 0 << '\n';
	}
	else{ //EE_EDGE

		int sourceStartIndex = sourceIndex;
		int sourceEndIndex = m_readsInfo.get_readLen(sourceVertex) -1;
		int sourceLength = m_readsInfo.get_readLen(sourceVertex);

		int destinationLength = m_readsInfo.get_readLen(destinationVertex);
		int destinationEndIndex = destinationLength - 1;
		int destinationStartIndex = destinationEndIndex - (sourceEndIndex - sourceStartIndex);



		m_asqgWriter << sourceStartIndex << " "
				<< sourceEndIndex << " "
				<< sourceLength << " "
				<< destinationStartIndex<< " "
				<< destinationEndIndex<< " "
				<< destinationLength<< " "
				<< 1 << " " << 0 << '\n';

	}

}

void constructCombinedGraph(){

	//construct (E,E) edges;
	Edges *pBE = get_edges_from_file("temp_be", FORWARD);
	Edges *pEE = get_edges_from_file("temp_ee", FORWARD);

	construct_edges(*pBE,*pEE, *pEE, "temp_ee_new");
	delete pEE
	//construct (B,B) edges
	Edges *pBER = get_revese_graph(*pBE);
	Edges *pBB = get_edges_from_file("temp_bb",FORWARD);

	construct_edges(*pBER, *pBB, *pBB, "temp_bb_new");

	delete *pBER;
	//construct (B,E) edges
	Edges *pEER = get_edges_from_file("temp_ee", REVERSE);
	construct_edges(*pEER, *pBB, "temp_be", "temp_be_new1");

	*pEE = get_reverse_graph(*pEER);
	*pBBR = get_reverse_graph(*pBB);
	construct_edges(*pEE, *pBBR, "temp_be_new1", "temp_be_new2");



}

} /* namespace sof */

