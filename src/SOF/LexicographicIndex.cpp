/* Bismillahir Rahmanir Rahim
 * LexicographicIndex.cpp
 *
 *  Created on: Jul 19, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#include "LexicographicIndex.h"

namespace sof {

LexicographicIndex::LexicographicIndex(std::string saiFileName)
		: 	m_ssa(SampledSuffixArray(saiFileName, SSA_FT_SAI)),
			m_ssaSize(m_ssa.getLexoIndexSize()) {
	//assert test
	int value = 10;
	assert(value == 11);

}

void LexicographicIndex::print() {

	std::cout<<"Lexicographic index: "<<'\n';
	for(numReads_t i=0; i< m_ssaSize; i++){
		std::cout<< i <<" -> "<<m_ssa.lookupLexoRank(i)<<'\n';
	}
}

} /* namespace sof */


