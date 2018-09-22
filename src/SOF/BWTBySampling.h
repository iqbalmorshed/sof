/*
 * BWTBySampling.h
 *
 *  Created on: Jun 18, 2018
 *      Author: iqbal
 */

#ifndef SRC_SOF_BWTBYSAMPLING_H_
#define SRC_SOF_BWTBYSAMPLING_H_

#include "BWT.h"
#include "RLBWT.h"

namespace sof {

class BWTBySampling: public BWT {
public:
	BWTBySampling(	const std::string& bwtFileName,
					const int sampleRate = RLBWT::DEFAULT_SAMPLE_RATE_SMALL);
	virtual ~BWTBySampling();

	//functions
	BWTInterval get_backward_interval(	const BWTInterval,
										const char ch) const override;
	TerminalInterval get_backward_terminal_interval(const BWTInterval) const
			override;
	void print() const override;

private:
	RLBWT *m_pBWT;

};

} /* namespace sof */

#endif /* SRC_SOF_BWTBYSAMPLING_H_ */
