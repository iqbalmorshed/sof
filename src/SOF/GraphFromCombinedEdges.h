/*
 * GraphFromCombinedEdges.h
 *
 *  Created on: Sep 16, 2018
 *      Author: iqbal
 */

#ifndef SRC_SOF_GRAPHFROMCOMBINEDEDGES_H_
#define SRC_SOF_GRAPHFROMCOMBINEDEDGES_H_

#include <vector>
#include <string>
#include <map>

#include "ReadsInfo.h"
#include "SOFCommon.h"
#include "StringGraph.h"
#include "FileWriter.h"


namespace sof {

enum EdgeDirection{
	FORWARD,
	REVERSE
};
struct DestinationVertexInfo{
	numReads_t destinationVertex;
	readLen_t overlapLength;
};

using Edges = std::vector<std::vector<DestinationVertexInfo> >;

class GraphFromCombinedEdges {
public:
	GraphFromCombinedEdges(const ReadsInfo& readsInfo, const readLen_t minOverlap);

private:
	void get_edges_from_file(const std::string& fileName,
								const EdgeDirection edgeDirection,
								Edges& edges);

	void construct_edges(const Edges& xyEdges,
			const EdgeDirection xyEdgeDirection,
			const Edges& yzEdges,
			const EdgeDirection yzEdgeDirection,
			const Edges& xzEdges,
			const std::string& tempFileName);

	void construct_edges(const Edges& xyEdges,
			const EdgeDirection xyEdgeDirection,
			const Edges& yzEdges,
			const EdgeDirection yzEdgeDirection,
			const std::string& xzEdgesFileName,
			const std::string& tempFileName);


	void write_in_temp_file(const numReads_t virtualID,
							const Edges& xzEdges,
							std::ofstream& tempFileWriter);

	void get_reverse_edges(const Edges& sourceEdges, Edges& reverseEdges);

	void mark_transitive(const numReads_t virtualID,
							const Edges& xyEdges,
							const Edges& yzEdges,
							const bool isXFullListSearch,
							const bool isYFullListSearch,
							const readLen_t xzMinOverlap);

	const ReadsInfo& m_readsInfo;
	const numReads_t m_numReads;
	const readLen_t m_minOverlap;
	std::vector<numReads_t> m_transitive;
};

} /* namespace sof */

#endif /* SRC_SOF_GRAPHFROMCOMBINEDEDGES_H_ */
