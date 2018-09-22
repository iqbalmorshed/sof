/*
 * ReadsInfo.h
 *
 *  Created on: Jun 24, 2018
 *      Author: iqbal
 */

#ifndef SRC_SOF_READSINFO_H_
#define SRC_SOF_READSINFO_H_

#include <vector>
#include <cassert>
#include "SOFCommon.h"
#include "SampledSuffixArray.h"
#include "BWT.h"

namespace sof {

class ReadsInfo {
public:
	ReadsInfo();

	inline numReads_t get_numReads() const {
		return m_numReads;
	}

	inline readLen_t get_readLen(numReads_t virtualID) const {
		assert(virtualID >=0 && virtualID < m_numReads);
		return m_readLen[virtualID];
	}

	inline bool get_isValid(numReads_t virtualID) const {
		//std::cout<<"virtualID:"<<virtualID<<"numReads:"<<m_numReads<<'\n';
		assert(virtualID >=0 && virtualID < m_numReads);
		return m_isValid[virtualID];
	}
	inline bool get_isVertex(numReads_t virtualID) const {
		assert(virtualID >=0 && virtualID < m_numReads);
		return m_isVertex[virtualID];
	}

//	inline numReads_t get_virtualID(numReads_t lexicoIndexID) const {
//
//		size_t castedIndex = static_cast<size_t>(lexicoIndexID);
//		return static_cast<numReads_t>(m_ssa.lookupLexoRank(castedIndex));
//	}

	inline void set_containers_size(numReads_t numReads){
		m_numReads = numReads;
		m_readLen = std::vector<readLen_t>(m_numReads);
		m_isValid = std::vector<bool>(m_numReads, true);
		m_isVertex = std::vector<bool>(m_numReads, false);
	}

	inline void set_readLen(numReads_t virtualID, readLen_t readLenValue){
		assert(virtualID >=0 && virtualID < m_numReads);
		m_readLen[virtualID] = readLenValue;
	}
	inline void set_isValid(numReads_t virtualID, bool bValue){
		assert(virtualID >=0 && virtualID < m_numReads);
		m_isValid[virtualID] = bValue;
	}
	inline void set_isVertex(numReads_t virtualID, bool bValue){
		assert(virtualID >=0 && virtualID < m_numReads);
		m_isVertex[virtualID] = bValue;
	}
	//release memory of m_isValid
	inline void clear_isValidVector(){
		std::vector<bool>().swap(m_isValid);
	}
	//release memory of m_isVertex
	inline void clear_isVertexVector(){
		std::vector<bool>().swap(m_isVertex);
	}


//	inline void set_lexicoIndex(numReads_t lexicoIndexID, numReads_t virtualID){
//		assert(lexicoIndexID >=0 && lexicoIndexID < m_numReads && virtualID >=0 && virtualID < m_numReads);
//		m_lexicoIndex[lexicoIndexID] = virtualID;
//	}

private:
	numReads_t m_numReads;

	//stores read length for a given virtual read id.
	std::vector<readLen_t> m_readLen;

	//stores whether the read is a valid read or not for a given read id.
	std::vector<bool> m_isValid;


	std::vector<bool> m_isVertex;

};

} /* namespace sof */

#endif /* SRC_SOF_READSINFO_H_ */
