/*
 * RepeatRemoval.h
 *
 *  Created on: Sep 17, 2018
 *      Author: iqbal
 */

#ifndef SRC_SOF_REPEATREMOVAL_H_
#define SRC_SOF_REPEATREMOVAL_H_

#include <string>
#include <vector>
#include <map>

#include "SOFCommon.h"

namespace sof {

class RepeatRemoval {
public:
	RepeatRemoval(numReads_t numReads);
private:
	void remove_repeat(const std::string& tempFileName);
	numReads_t m_numReads;
};

} /* namespace sof */

#endif /* SRC_SOF_REPEATREMOVAL_H_ */
