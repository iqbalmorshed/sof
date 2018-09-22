/*
 * CurrentRead.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: iqbal
 */

#include "CurrentRead.h"

namespace sof {

CurrentRead::CurrentRead(	numReads_t virtualID,
							const OverlapInfoVector& readOverlaps,
							readLen_t readLength,
							readLen_t minOverlap)
		: 	m_virtualID(virtualID),
			m_maxIndex(readLength - minOverlap) {

	//m_indexedReadOverlaps = std::vector<IntervalVector>(m_maxIndex+1);
	m_indexedReadOverlaps = std::vector<IntervalList>(m_maxIndex + 1);
	m_nElementsInIndex = std::vector<numReads_t>(m_maxIndex + 1, 0);

	for (auto &overlapInfo : readOverlaps) {
		m_indexedReadOverlaps[overlapInfo.readIndex].push_back(
				overlapInfo.terminalInterval);

		m_nElementsInIndex[overlapInfo.readIndex]++;

		if (m_nElementsInIndex[overlapInfo.readIndex] == 1) {
			m_popIndexVector.push_back(overlapInfo.readIndex);
		}
	}

}

bool CurrentRead::is_poppable_interval() {

	while (!m_popIndexVector.empty()) {
		readLen_t index = m_popIndexVector.back();
		m_popIndexVector.pop_back();

		if (m_nElementsInIndex[index]) {
			m_popIndex = index;
			return true;
		}

	}
	return false;
}

void CurrentRead::get_all_irreducible_intervals(OverlapInfoVector& overlapInfoVector) const {

	OverlapInfo overlapInfo;
	for (readLen_t i = 0; i <= m_maxIndex; i++) {
		if (m_nElementsInIndex[i]) {
			for (auto &element : m_indexedReadOverlaps[i]) {
				overlapInfo.terminalInterval = element;
				overlapInfo.readIndex = i;
				overlapInfoVector.push_back(overlapInfo);
			}
		}
	}

}

PoppedInterval CurrentRead::pop_intervals_from_lowest_index() const {

	PoppedInterval poppedInterval;
	IntervalVector intervalVector;

	for (auto &interval : m_indexedReadOverlaps[m_popIndex]) {
		poppedInterval.intervalVector.push_back(interval);
	}
	poppedInterval.index = m_popIndex;
	return poppedInterval;
}

} /* namespace sof */

void sof::CurrentRead::split_interval(	readLen_t index,
										TerminalInterval givenInterval) {

	if (m_nElementsInIndex[index]) {
		auto it = m_indexedReadOverlaps[index].begin();

		while (it != m_indexedReadOverlaps[index].end()
				&& givenInterval.lower > it->lower) {

			if (givenInterval.lower <= it->upper) {

				if (givenInterval.upper == it->upper) {
					it->upper = givenInterval.lower - 1;
					return;
				} else if (givenInterval.upper > it->upper) {
					it->upper = givenInterval.lower - 1;
				} else { //givenInterval.upper < it->upper
					TerminalInterval newInterval;
					newInterval.lower = givenInterval.upper + 1;
					newInterval.upper = it->upper;

					it->upper = givenInterval.lower - 1;

					it++;
					m_indexedReadOverlaps[index].insert(it, newInterval);
					m_nElementsInIndex[index]++;
					return;
				}
			}
			it++;
		}
		//givenInterval.lower <= it->lower
		while (it != m_indexedReadOverlaps[index].end() && givenInterval.upper >= it->lower ) {

			if (givenInterval.upper == it->upper) {
				m_indexedReadOverlaps[index].erase(it);
				m_nElementsInIndex[index]--;
				return;
			} else if (givenInterval.upper > it->upper) {
				it= m_indexedReadOverlaps[index].erase(it);
				m_nElementsInIndex[index]--;
			} else{
				it->lower = givenInterval.upper + 1;
				return;
			}

		}

	}

}

void sof::CurrentRead::print_intervals() {

	std::cout << "In read:" << m_virtualID << '\n';
	for (readLen_t i = 0; i <= m_maxIndex; i++) {
		std::cout << static_cast<int>(i) << "-->";
		if (m_nElementsInIndex[i]) {
			for (auto it = m_indexedReadOverlaps[i].begin();
					it != m_indexedReadOverlaps[i].end(); it++) {
				std::cout << "(" << it->lower << "," << it->upper << ") ";
			}
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}
/*
 split_interval_using_vector(readLen_t index,
 TerminalInterval givenInterval){

 IntervalVector &intervalVector = m_indexedReadOverlaps[index];
 //get interval index by performing binary search.
 numReads_t intervalIndex =get_interval_index();

 //checks whether there is overlap between given interval and interval in intrevalIndex-1
 if(intervalIndex>0 && givenInterval.upper <= intervalVector[intervalIndex -1].lower ){

 if(givenInterval.upper == intervalVector[intervalIndex-1].upper){
 intervalVector[intervalIndex-1].upper = givenInterval.lower -1;
 return;
 }
 else if(givenInterval.upper > intervalVector[intervalIndex-1].upper){
 intervalVector[intervalIndex-1].upper = givenInterval.lower -1;
 }
 else{
 TerminalInterval newInterval;
 newInterval.lower = givenInterval.upper+1;
 newInterval.upper = intervalVector[intervalIndex-1].upper;

 intervalVector[intervalIndex-1].upper = givenInterval.lower-1;
 insert
 return;
 }

 }


 }
 */
