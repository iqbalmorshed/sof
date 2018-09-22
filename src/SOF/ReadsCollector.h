/* Bismillahir Rahmanir Rahim
 * ReadsCollector.h
 *
 *  Created on: Jul 20, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#ifndef SRC_SOF_READSCOLLECTOR_H_
#define SRC_SOF_READSCOLLECTOR_H_

#include <iostream>
#include "SOFCommon.h"

namespace sof {

struct ReadData{
	std::string seq;
	readLen_t length;
};

class ReadsCollector {
public:
	ReadsCollector();

	void build_bwt(std::string readsFileName, std::string bwtFileName);
	void collect_read_from_BWT(std::string bwtFileName);
	void collect_read_from_fastq(std::string readsFileName);

private:
};

} /* namespace sof */

#endif /* SRC_SOF_READSCOLLECTOR_H_ */
