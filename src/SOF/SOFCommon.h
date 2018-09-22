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

enum EdgeType{
	BE_EDGE = 0,
	BB_EDGE,
	EE_EDGE,
};

using numReads_t =  int; //int64_t; // TODO
using numBases_t = int_fast64_t;
using readLen_t = int16_t;
using isDone_t = bool;

#define DEFAULT_REPEAT_PRESENCE false
#define DEFAULT_SUBSTRING_PRESENCE false

#endif /* SRC_SOF_SOFCOMMON_H_ */
