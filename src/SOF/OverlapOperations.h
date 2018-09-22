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
#include "OverlapContainer.h"

namespace sof {

class OverlapOperations {

public:
	OverlapOperations();

	//computes the terminal-intervals (if there is any) for each sub-string in
	//"sequence" starting
	//from sequence.length()-1 upto index 1. BWT Interval for the 2nd index
	//(i.e. index 1) is returned. Result is stored in the overlaps
	BWTInterval get_overlaps(	const BWT* pBWT,
								const std::string& sequence,
								const readLen_t minOverlap,
								OverlapInfoVector& overlapInfoVector);

};

} /* namespace sof */

#endif /* SRC_SOF_OVERLAPOPERATIONS_H_ */
