/*
 * OperationOnBE.h
 *
 *  Created on: Jun 23, 2018
 *      Author: iqbal
 */

#ifndef SRC_SOF_OPERATIONONBE_H_
#define SRC_SOF_OPERATIONONBE_H_

#include "ReadOperations.h"
#include "CurrentRead.h"
#include "LexicographicIndex.h"

namespace sof {

class OperationOnBE: public ReadOperations {
public:
	OperationOnBE(	const BWT* pBWT,
					const OverlapContainer& overlapContainer,
					const LexicographicIndex& lexicoIndex,
					const readLen_t minOverlap,
					ReadsInfo& readsInfo);
	virtual ~OperationOnBE();

	virtual CurrentRead get_read(numReads_t virtualReadID) override;
	virtual void write_edges(CurrentRead& currentRead) override;

	inline bool isBothReverseComplement(bool isSourceRevComp, numReads_t virtualID){
		return isSourceRevComp && virtualID%2;
	}

};

} /* namespace sof */

#endif /* SRC_SOF_OPERATIONONBE_H_ */
