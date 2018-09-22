/* Bismillahir Rahmanir Rahim
 * OperationOnBB.h
 *
 *  Created on: Jul 22, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#ifndef SRC_SOF_OPERATIONONBBANDEE_H_
#define SRC_SOF_OPERATIONONBBANDEE_H_

#include "ReadOperations.h"
#include "StringOperations.h"

namespace sof {

class OperationOnBBandEE: public ReadOperations {
public:
	OperationOnBBandEE(	const BWT* pBWT,
					const OverlapContainer& overlapContainer,
					const LexicographicIndex& lexicoIndex,
					const readLen_t minOverlap,
					ReadsInfo& readsInfo,
					const std::string& readsFileName,
					const EdgeType edgeType);
	virtual ~OperationOnBBandEE();

	virtual CurrentRead get_read(numReads_t virtualID) override;
	virtual void write_edges(CurrentRead& currentRead) override;

private:
	const std::string& m_readsFileName;
	SeqReader m_seqReader;
	const EdgeType m_edgeType;
	numReads_t m_nReadsCollected = 0;
	StringOperations m_stringOperations;

};

} /* namespace sof */

#endif /* SRC_SOF_OPERATIONONBBANDEE_H_ */
