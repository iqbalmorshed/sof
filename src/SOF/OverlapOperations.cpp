/* Bismillahir Rahmanir Rahim
 * OverlapOperations.cpp
 *
 *  Created on: Jul 22, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#include "OverlapOperations.h"

namespace sof {

OverlapOperations::OverlapOperations() {
	// TODO Auto-generated constructor stub

}

BWTInterval OverlapOperations::get_overlaps(const BWT* pBWT,
											const std::string& sequence,
											const readLen_t minOverlap,
											OverlapInfoVector& overlapInfoVector) {
	readLen_t seqLength = sequence.length();
	BWTInterval bwtInterval;
	TerminalInterval terminalInterval;
	OverlapInfo overlapInfo;

	bwtInterval.lower = 0;
	bwtInterval.upper = pBWT->get_bwtLength() - 1;

	for (readLen_t seqIndex = seqLength - 1; seqIndex >= 1; seqIndex--) {

		char base = sequence[seqIndex];
		bwtInterval = pBWT->get_backward_interval(bwtInterval, base);
		//terminalInterval = pBWT->get_backward_terminal_interval(bwtInterval);

		if (seqIndex <= seqLength - minOverlap) {

			terminalInterval = pBWT->get_backward_terminal_interval(bwtInterval);

			if(terminalInterval.lower <= terminalInterval.upper){
				overlapInfo.terminalInterval = terminalInterval;
				overlapInfo.readIndex = seqIndex;

				overlapInfoVector.push_back(overlapInfo);
			}
		}
	}
	return bwtInterval;
}

}
