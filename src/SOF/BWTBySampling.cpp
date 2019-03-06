/*
 * BWTBySampling.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: iqbal
 */

#include "BWTBySampling.h"
#include <iostream>

namespace sof {

} /* namespace sof */

sof::BWTBySampling::BWTBySampling(	const std::string& bwtFileName,
									const int sampleRate)
		: BWT(bwtFileName) {
	m_pBWT = new RLBWT(bwtFileName, sampleRate);
	m_bwtLength = static_cast<numBases_t>(m_pBWT->getBWLen());
	m_numReads = static_cast<numReads_t>(m_pBWT->getNumStrings());
	m_bwtSizeInBytes = static_cast<numReads_t>(m_pBWT->getBWTSizeInBytes());

//	std::cout<<"Inside BWTBySampling Constructor: m_bwtLength: "<<m_bwtLength<<" m_pBWT->get:"<< m_pBWT->getBWLen()<<'\n';
//	std::cout<<"Inside BWTBySampling Constructor: m_numReads: "<<m_numReads<<" m_pBWT->getNum:"<< m_pBWT->getNumStrings()<<'\n';


}

sof::TerminalInterval sof::BWTBySampling::get_backward_terminal_interval(const BWTInterval bwtInterval) const {

	BWTInterval newBWTInterval;
	TerminalInterval terminalInterval;
	newBWTInterval = get_backward_interval(bwtInterval, '$');

	terminalInterval.lower = newBWTInterval.lower;
	terminalInterval.upper = newBWTInterval.upper;
	return terminalInterval;
}

void sof::BWTBySampling::print() const {
	m_pBWT->print();
}
void sof::BWTBySampling::printInfo() const {
	m_pBWT->printInfo();
}
sof::BWTBySampling::~BWTBySampling() {
	delete m_pBWT;
}

sof::BWTInterval sof::BWTBySampling::get_backward_interval(	const BWTInterval bwtInterval,
															const char ch) const {

//	std::cout<<"bwtInterval: char: "<<ch<<" lower: "<<bwtInterval.lower<<" upper: "<<bwtInterval.upper<<'\n';
	BWTInterval newBWTInterval;
	AlphaCount64 l = m_pBWT->getFullOcc(bwtInterval.lower - 1);
	AlphaCount64 u = m_pBWT->getFullOcc(bwtInterval.upper);

	size_t pb = m_pBWT->getPC(ch);
	newBWTInterval.lower = pb + l.get(ch);
	newBWTInterval.upper = pb + u.get(ch) - 1;
	return newBWTInterval;

}


