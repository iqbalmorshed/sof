/*
 * BWT.h
 *
 *  Created on: Jun 17, 2018
 *      Author: iqbal
 */

#ifndef SRC_SOF_BWT_H_
#define SRC_SOF_BWT_H_

#include <iostream>
#include "SOFCommon.h"

namespace sof {

struct BWTInterval{
	//indexing starts from 0.
	numBases_t lower;
	numBases_t upper;
};

class BWT {
public:
	BWT(const std::string& bwtFileName);
	virtual ~BWT();
	virtual BWTInterval get_backward_interval(const BWTInterval,const char ch)const = 0;
	virtual TerminalInterval get_backward_terminal_interval(const BWTInterval)const = 0;
	virtual void print()const = 0;
	virtual void printInfo()const = 0;

	inline numBases_t get_bwtLength()const {return m_bwtLength;}
	inline numReads_t get_numReads()const {return m_numReads;}

	inline int getBwtSizeInBytes() const {
		return m_bwtSizeInBytes;
	}

protected:
	const std::string& m_bwtFileName;
	numBases_t m_bwtLength;
	numReads_t m_numReads;
	int m_bwtSizeInBytes;

private:


};

} /* namespace sof */

#endif /* SRC_SOF_BWT_H_ */
