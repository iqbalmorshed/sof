/*
 * BWT.cpp
 *
 *  Created on: Jun 17, 2018
 *      Author: iqbal
 */

#include "BWT.h"

namespace sof {

BWT::BWT(const std::string& bwtFileName)
	: m_bwtFileName(bwtFileName),
	  m_bwtLength(0),
	  m_numReads(0){

}

BWT::~BWT() {
}

} /* namespace sof */
