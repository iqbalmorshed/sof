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

#include "SOFCommon.h"

using pii = std::pair<int, int>;

class Edge{
public:
	numReads_t sourceVertex;
	numReads_t destinationVertex;

	friend bool operator== (const Edge &c1, const Edge &c2){
	    return (c1.sourceVertex== c2.sourceVertex &&
	            c1.destinationVertex== c2.destinationVertex);

	}
};


namespace sof {

//using EdgeMap = std::map<numReads_t, readLen_t> ;
using EdgeMap = SparseHashMap<pii, readLen_t> ;

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
//			m_edge.sourceVertex = sourceVertexID;
//			m_edge.destinationVertex = destinationVertexID;
			pii edge(sourceVertexID, destinationVertexID);
			m_edgeInfoMap[edge] = sourceIndex;
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
//			m_edge.sourceVertex = destinationVertexID;
//			m_edge.destinationVertex = sourceVertexID;
			pii edge(destinationVertexID, sourceVertexID);
			return (m_edgeInfoMap.find(edge)!= m_edgeInfoMap.end() &&
					m_edgeInfoMap[edge]==overlapLength);
		}
	}

//	void clear_container(){
//		std::vector<EdgeMap >().swap(m_edgeInfoMap);
//	}

private:

	EdgeMap m_edgeInfoMap;
	pii m_edge;
};

} /* namespace sof */

#endif /* SRC_SOF_EDGECONTAINER_H_ */
