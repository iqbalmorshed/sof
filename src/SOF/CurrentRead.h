/*
 * CurrentRead.h
 *
 *  Created on: Jun 18, 2018
 *      Author: iqbal
 */

#ifndef SRC_SOF_CURRENTREAD_H_
#define SRC_SOF_CURRENTREAD_H_

#include <vector>

#include "OverlapContainer.h"

namespace sof {

using OverlapInfoVector = std::vector<OverlapInfo>;
using IntervalVector = std::vector<TerminalInterval>;
using IntervalList = std::list<TerminalInterval>;

struct PoppedInterval {
	IntervalVector intervalVector;
	readLen_t index;
};

class CurrentRead {

public:
//	CurrentRead(numReads_t virtualID,				const OverlapInfoVector& readOverlaps,
//				readLen_t readLength,
//				readLen_t minOverlap);
	CurrentRead(){}

	void split_interval(readLen_t index,
						TerminalInterval terminalInterval);

	//bool is_empty(readLen_t index) const;
	bool is_poppable_interval();

	PoppedInterval pop_intervals_from_lowest_index() const;
	void get_all_irreducible_intervals(OverlapInfoVector& overlapInfoVector) const;

	void print_intervals();
	void print_intervals_in_index(readLen_t index);

	readLen_t m_maxIndex;
	numReads_t m_virtualID;



	//std::vector<IntervalVector> m_indexedReadOverlaps;
	std::vector<IntervalList> m_indexedReadOverlaps;
	std::vector<int> m_nElementsInIndex;

	std::vector<readLen_t> m_popIndexVector;

private:
	readLen_t m_popIndex = 0;

};

} /* namespace sof */

#endif /* SRC_SOF_CURRENTREAD_H_ */
