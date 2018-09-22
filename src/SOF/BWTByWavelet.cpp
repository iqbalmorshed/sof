/* Bismillahir Rahmanir Rahim
 * BWTByWavelet.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#include "BWTByWavelet.h"

#include <sdsl/wavelet_trees.hpp>

#include "RLBWT.h"

namespace sof {

BWTByWavelet::BWTByWavelet(const std::string& bwtFileName)
		: BWT(bwtFileName) {
	m_pBWT = new RLBWT(bwtFileName);
	m_bwtLength = m_pBWT->getBWLen();
	m_numReads = m_pBWT->getNumStrings();

	set_order_for_alphabet();
	construct_c_arr();

	m_pBWT->writeBWTToFile("temp_bwt_file.txt");
	delete m_pBWT;

	m_wtBWT = sdsl::wt_huff<>();
	construct(m_wtBWT, "temp_bwt_file.txt", 1);

}

BWTByWavelet::~BWTByWavelet() {
	// TODO Auto-generated destructor stub
}

BWTInterval BWTByWavelet::get_backward_interval(const BWTInterval bwtInterval,
												const char ch) const {

	assert(bwtInterval.lower >= 0 && bwtInterval.lower < m_bwtLength
			&& bwtInterval.upper >= 0 && bwtInterval.upper < m_bwtLength);

	BWTInterval newBWTInterval;
	newBWTInterval.lower = m_C[m_order[ch]]
			+ m_wtBWT.rank(bwtInterval.lower, ch);
//	std::cout<<"lower : c[ch]"<<m_C[m_order[ch]]<<" wt rank: "<<m_wtBWT.rank(bwtInterval.lower, ch)<<'\n';
	newBWTInterval.upper = m_C[m_order[ch]]
			+ m_wtBWT.rank(bwtInterval.upper + 1, ch)-1;
//	std::cout<<"upper : c[ch]"<<m_C[m_order[ch]]<<" wt rank: "<<m_wtBWT.rank(bwtInterval.upper + 1, ch) - 1<<'\n';

	return newBWTInterval;
}

TerminalInterval BWTByWavelet::get_backward_terminal_interval(const BWTInterval bWTInterval) const {

	TerminalInterval terminalInterval;
	BWTInterval BWTInterval = get_backward_interval(bWTInterval, '$');
	;
	terminalInterval.lower = BWTInterval.lower;
	terminalInterval.upper = BWTInterval.upper;
	return terminalInterval;

}

void BWTByWavelet::print() const {

	std::cout << "BWT String: ";
	for (numBases_t i; i < m_bwtLength; i++) {
		std::cout << m_wtBWT[i] << " ";
	}
	std::cout << '\n';
}

void BWTByWavelet::set_order_for_alphabet() {
	int len = m_DNAalphabet.length();
	int order = 0;
	for (int i = 0; i < len; i++) {
		m_order[m_DNAalphabet[i]] = order;
		order++;
	}
}

void BWTByWavelet::construct_c_arr() {
	int len = m_DNAalphabet.length();
	for (int i = 0; i < len; i++) {
		m_C[m_order[m_DNAalphabet[i]]] = m_pBWT->getPC(m_DNAalphabet[i]);
	}
}

} /* namespace sof */
