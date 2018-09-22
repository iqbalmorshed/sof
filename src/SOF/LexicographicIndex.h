/* Bismillahir Rahmanir Rahim
 * LexicographicIndex.h
 *
 *  Created on: Jul 19, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#ifndef SRC_SOF_LEXICOGRAPHICINDEX_H_
#define SRC_SOF_LEXICOGRAPHICINDEX_H_

#include "SampledSuffixArray.h"
#include "SOFCommon.h"

namespace sof {

class LexicographicIndex {
public:
	LexicographicIndex(std::string saiFileName);

	//returns virtualID for the corresponding lexicoID
	inline numReads_t operator[](numReads_t lexicoID) const {
		assert(lexicoID>=0 && lexicoID < m_ssaSize);
		size_t castedIndex = static_cast<size_t>(lexicoID);
		return static_cast<numReads_t>(m_ssa.lookupLexoRank(castedIndex));
	}
	void print();
private:
	const SampledSuffixArray m_ssa;
	const numReads_t m_ssaSize;

};

} /* namespace sof */

#endif /* SRC_SOF_LEXICOGRAPHICINDEX_H_ */
