/*
 * CombinedGraph.h
 *
 *  Created on: Sep 6, 2018
 *      Author: iqbal
 */

#ifndef SRC_SOF_COMBINEDGRAPH_H_
#define SRC_SOF_COMBINEDGRAPH_H_

#include <vector>
#include <string>
#include <map>

#include "ReadsInfo.h"
#include "SOFCommon.h"
#include "StringGraph.h"

namespace sof {

enum CombinedEdgeType{
	BE_EDGE_FD = 0,
	BB_EDGE_FD,
	EE_EDGE_FD,
	BE_EDGE_RV,
	BB_EDGE_RV,
	EE_EDGE_RV
};

struct DestinationVertexInfo{
	numReads_t destinationVertex;
	readLen_t overlapLength;
};

using EdgeInformation = std::vector< std::vector<DestinationVertexInfo> >;
using EdgeInfoMap = std::vector<std::map<numReads_t, readLen_t> >;

class EdgeStorage{
public:
	EdgeStorage(){
	}
	EdgeStorage(const numReads_t& numReads){
		m_edgeInfoMap = std::vector<EdgeInfoMap>(numReads, EdgeInfoMap(2, std::map<numReads_t, readLen_t>()));
	}

	void set_edge(EdgeType edgeType,
			numReads_t sourceVertexID,
			numReads_t destinationVertexID,
			readLen_t sourceIndex){
		m_edgeInfoMap[sourceVertexID][edgeType -1][destinationVertexID]=sourceIndex;
	}

	bool is_present(EdgeType edgeType,
					numReads_t sourceVertexID,
					numReads_t destinationVertexID,
					readLen_t sourceIndex){
		return m_edgeInfoMap[sourceVertexID][edgeType -1].find(destinationVertexID) !=
				m_edgeInfoMap[sourceVertexID][edgeType -1].end() &&
				m_edgeInfoMap[sourceVertexID][edgeType -1][destinationVertexID]==sourceIndex;
	}

class CombinedGraph {
public:
	CombinedGraph(const InputData& inputData,
			const ReadsInfo& readsInfo);
	void prune();
private:

	void collect_edges(const std::string& tempFileName,const EdgeType edgeType);
	void find_transitive_edges(const numReads_t virtualID,
							const CombinedEdgeType xyEdgeType,
							const CombinedEdgeType yzEdgeType,
							const readLen_t xzMinOverlap);

	void write_irriducible_edges(numReads_t virtualID, EdgeType edgeType);
	void create_asqg_header_and_vertices();
	void write_edges_in_asqg(const EdgeType edgeType,
			const numReads_t sourceVertex,
			const numReads_t destinationVertex,
			const readLen_t sourceIndex);

	const InputData& m_inputData;
	const ReadsInfo& m_readsInfo;
	const numReads_t m_numReads;

	std::vector< EdgeInformation> m_adjList;
	std::vector<numReads_t> m_transitive;
	std::ofstream m_asqgWriter;
	std::vector<std::string> m_realIDVector;
	EdgeStorage m_edgeStore;

};

} /* namespace sof */

#endif /* SRC_SOF_COMBINEDGRAPH_H_ */
