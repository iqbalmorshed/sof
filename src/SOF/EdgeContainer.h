/*
 * EdgeContainer.h
 *
 *  Created on: Sep 17, 2018
 *      Author: iqbal
 */

#ifndef SRC_SOF_EDGECONTAINER_H_
#define SRC_SOF_EDGECONTAINER_H_

/*
 * EdgeContainer only containes edges where sourceVertex < destination vertex
 * This helps to identify repeats in the edge set.
*/
#include <vector>
#include <map>
#include "HashMap.h"
#include "spp.h"

#include "SOFCommon.h"

using pii = std::pair<numReads_t, numReads_t>;

//class Edge{
//public:
//	numReads_t sourceVertex;
//	numReads_t destinationVertex;
//
//	friend bool operator== (const Edge &c1, const Edge &c2){
//	    return (c1.sourceVertex== c2.sourceVertex &&
//	            c1.destinationVertex== c2.destinationVertex);
//
//	}
//};
//
//struct hash_pair {
//    template <class T1, class T2>
//    size_t operator()(const std::pair<T1, T2>& p) const
//    {
//        auto hash1 = std::hash<T1>{}(p.first);
//        auto hash2 = std::hash<T2>{}(p.second);
//        return hash1 ^ hash2;
//    }
//};

struct Edge
{
    bool operator==(const Edge &o) const
    { return sourceVertex == o.sourceVertex && destinationVertex == o.destinationVertex; }

	numReads_t sourceVertex;
	numReads_t destinationVertex;
};

namespace std
{
    // inject specialization of std::hash for Person into namespace std
    // ----------------------------------------------------------------
    template<>
    struct hash<Edge>
    {
        std::size_t operator()(Edge const &p) const
        {
            std::size_t seed = 0;
            spp::hash_combine(seed, p.sourceVertex);
            spp::hash_combine(seed, p.destinationVertex);
            return seed;
        }
    };
}

namespace sof {

//using EdgeMap = std::map<numReads_t, readLen_t> ;
//using EdgeMap = SparseHashMap<pii, readLen_t, hash_pair> ;
using EdgeMap = spp::sparse_hash_map<Edge, readLen_t> ;

class EdgeContainer {
public:
	EdgeContainer();

	EdgeContainer(const numReads_t& numReads){
		//m_edgeInfoMap = std::vector<EdgeMap>(numReads/2);
		m_edgeInfoMap = EdgeMap();
	}

	void set_edge(numReads_t sourceVertexID,
				numReads_t destinationVertexID,
				readLen_t sourceIndex){

		if(sourceVertexID < destinationVertexID){
			//m_edgeInfoMap[sourceVertexID][destinationVertexID]=sourceIndex;
			m_edge.sourceVertex = sourceVertexID;
			m_edge.destinationVertex = destinationVertexID;
//			pii edge(sourceVertexID, destinationVertexID);
			m_edgeInfoMap[m_edge] = sourceIndex;
		}

	}

	bool is_present(numReads_t sourceVertexID,
					numReads_t destinationVertexID,
					readLen_t overlapLength){
		if(sourceVertexID < destinationVertexID)
			return false;
		else{
//			return (m_edgeInfoMap[destinationVertexID].find(sourceVertexID) !=
//				m_edgeInfoMap[destinationVertexID].end() &&
//				m_edgeInfoMap[destinationVertexID][sourceVertexID]==overlapLength);
			m_edge.sourceVertex = destinationVertexID;
			m_edge.destinationVertex = sourceVertexID;
//			pii edge(destinationVertexID, sourceVertexID);
			return (m_edgeInfoMap.find(m_edge)!= m_edgeInfoMap.end() &&
					m_edgeInfoMap[m_edge]==overlapLength);
		}
	}

//	void clear_container(){
//		std::vector<EdgeMap >().swap(m_edgeInfoMap);
//	}

private:

	EdgeMap m_edgeInfoMap;
	Edge m_edge;
	//pii m_edge;
};

} /* namespace sof */

#endif /* SRC_SOF_EDGECONTAINER_H_ */
