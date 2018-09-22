/* Bismillahir Rahmanir Rahim
 * BWTByWavelet.h
 *
 *  Created on: Jul 30, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#ifndef SRC_SOF_BWTBYWAVELET_H_
#define SRC_SOF_BWTBYWAVELET_H_

#include <sdsl/wavelet_trees.hpp>

#include "BWT.h"
#include "RLBWT.h"


namespace sof {

class BWTByWavelet: public BWT {
public:
	BWTByWavelet(const std::string& bwtFileName);
	virtual ~BWTByWavelet();

	//functions
	BWTInterval get_backward_interval(	const BWTInterval,
										const char ch) const override;
	TerminalInterval get_backward_terminal_interval(const BWTInterval) const
			override;
	void print() const override;

private:
	void set_order_for_alphabet();
	void construct_c_arr();

	const RLBWT *m_pBWT;
	sdsl::wt_huff<> m_wtBWT;
	std::string m_DNAalphabet = "$ACGT";
	numBases_t m_C[5];
	int m_order[128]; //can contain the order of all ascii charecters.

};

} /* namespace sof */

#endif /* SRC_SOF_BWTBYWAVELET_H_ */
