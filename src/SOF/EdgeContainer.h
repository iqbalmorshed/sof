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

#include "SOFCommon.h"

namespace sof {

using EdgeMap = std::map<numReads_t, readLen_t> ;

class EdgeContainer {
public:
	EdgeContainer();

	EdgeContainer(const numReads_t& numReads){
		m_edgeInfoMap = std::vector<EdgeMap>(numReads/2);
	}

	void set_edge(numReads_t sourceVertexID,
				numReads_t destinationVertexID,
				readLen_t sourceIndex){

		if(sourceVertexID < destinationVertexID)
			m_edgeInfoMap[sourceVertexID][destinationVertexID]=sourceIndex;

	}

	bool is_present(numReads_t sourceVertexID,
					numReads_t destinationVertexID,
					readLen_t overlapLength){
		if(sourceVertexID < destinationVertexID)
			return false;
		else
			return (m_edgeInfoMap[destinationVertexID].find(sourceVertexID) !=
				m_edgeInfoMap[destinationVertexID].end() &&
				m_edgeInfoMap[destinationVertexID][sourceVertexID]==overlapLength);
	}

	void clear_container(){
		std::vector<EdgeMap >().swap(m_edgeInfoMap);
	}

private:

	std::vector<EdgeMap > m_edgeInfoMap;
};

} /* namespace sof */

#endif /* SRC_SOF_EDGECONTAINER_H_ */
