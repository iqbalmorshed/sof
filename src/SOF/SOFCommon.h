/* Bismillahir Rahmanir Rahim
 * SOFCommon.h
 *
 *  Created on: Jun 25, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#ifndef SRC_SOF_SOFCOMMON_H_
#define SRC_SOF_SOFCOMMON_H_

#define NDEBUG
#include <stdint.h>
#include <assert.h>
#include <vector>
#include <cstdio>

enum EdgeType{
	BE_EDGE = 0,
	BB_EDGE,
	EE_EDGE,
};

using numReads_t =  int; //int64_t; // TODO
using numBases_t = int_fast64_t;
using readLen_t = int16_t;
using isDone_t = bool;

namespace sof {

struct TerminalInterval{
	//indexing starts from 0.
	numReads_t lower;
	numReads_t upper;
};

struct OverlapInfo {
	TerminalInterval terminalInterval;
	readLen_t readIndex;
};

struct ChunkInfo{
	int ID = -1;
	numReads_t start = -1;
	numReads_t end = -1;
};

struct BufferInfo{
	char buffer[BUFSIZ];
	int charsCollectedInBuffer = 0;
	int totalCharsInBuffer = 0;
	numReads_t item =0;
	int itemNoInLine = 0;

};
using OverlapInfoVector = std::vector<OverlapInfo>;
} /* namespace sof */




#define DEFAULT_REPEAT_PRESENCE false
#define DEFAULT_SUBSTRING_PRESENCE false

#endif /* SRC_SOF_SOFCOMMON_H_ */
