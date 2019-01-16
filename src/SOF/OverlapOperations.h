/* Bismillahir Rahmanir Rahim
 * OverlapOperations.h
 *
 *  Created on: Jul 22, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#ifndef SRC_SOF_OVERLAPOPERATIONS_H_
#define SRC_SOF_OVERLAPOPERATIONS_H_

#include <vector>
#include "BWT.h"
#include "SOFCommon.h"
#include "ReadsInfo.h"

namespace sof {
using OverlapInfoVector = std::vector<OverlapInfo>;

class OverlapOperations {

public:
	OverlapOperations();

	//computes the terminal-intervals (if there is any) for each sub-string in
	//"sequence" starting
	//from sequence.length()-1 upto index 1. BWT Interval for the 2nd index
	//(i.e. index 1) is returned. Result is stored in the overlaps
	BWTInterval write_overlaps(	const BWT* pBWT,
								const std::string& sequence,
								const readLen_t minOverlap,
								const numReads_t virtualID,
								std::ofstream& overlapWriter);
	void read_overlaps(	std::ifstream& overlapReader,
						std::vector<OverlapInfoVector>& container,
						const ReadsInfo& readsInfo);

	BWTInterval get_overlaps(	const BWT* pBWT,
								const std::string& sequence,
								const readLen_t minOverlap,
								OverlapInfoVector& overlapInfoVector);

	int64_t getOverlapReadCount() const {
		return m_overlapReadCount;
	}

	int64_t getOverlapWriteCount() const {
		return m_overlapWriteCount;
	}

private:
	int64_t m_overlapReadCount = 0;
	int64_t m_overlapWriteCount = 0;

};

} /* namespace sof */

#endif /* SRC_SOF_OVERLAPOPERATIONS_H_ */
