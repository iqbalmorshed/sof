/* Bismillahir Rahmanir Rahim
 * ReadsCollector.cpp
 *
 *  Created on: Jul 20, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#include "ReadsCollector.h"

#include <fstream>

#include "RLBWT.h"
#include "BWTCARopebwt.h"
#include "SeqReader.h"
#include "Timer.h"
#include "Util.h"



namespace sof {

ReadsCollector::ReadsCollector(){
	// TODO Auto-generated constructor stub

}

void ReadsCollector::collect_read_from_fastq(std::string readsFileName) {

	Timer t("Time needed to generate reads from fastq file:"+readsFileName);
	std::ofstream outFile(readsFileName+".out2");


	SeqReader reader(readsFileName, SRF_NO_VALIDATION);
	SeqRecord record;

	//std::cout<<"inside container constructor:"<<std::endl;
	//numReads_t virtualID = 0;
	while (reader.get(record)) {

		std::string readSeq = record.seq.toString();
		outFile <<readSeq<<'\n';
	}
}

} /* namespace sof */

void sof::ReadsCollector::collect_read_from_BWT(std::string bwtFileName) {

	Timer t1("Timer t1: Starts before BWT file"+bwtFileName+ "loading to memory:");
	std::ofstream outFile(bwtFileName+".out");
	RLBWT* pBWT = new RLBWT(bwtFileName);

	int64_t numStrings = pBWT->getNumStrings();
	//m_saLexoIndex.resize(numStrings);
	//int64_t MAX_ELEMS = std::numeric_limits<SSA_INT_TYPE>::max();
	//assert(numStrings < MAX_ELEMS);
	Timer t2("Timer t2: Starts after BWT"+bwtFileName+ "loaded into memory:");

	for(int64_t read_idx = 0; read_idx < numStrings; ++read_idx)
	    {
			std::string readSeq;
	        // For each read, start from the end of the read and backtrack through the suffix array/BWT
	        // to calculate its lexicographic rank in the collection
	        size_t idx = read_idx;
	        while(1)
	        {
	            char b = pBWT->getChar(idx);
	            readSeq+= b;

	            idx = pBWT->getPC(b) + pBWT->getOcc(b, idx - 1);
	            if(b == '$')
	            {
	                // There is a one-to-one mapping between read_index and the element
	                // of the array that is set - therefore we can perform this operation
	                // without a lock.
	                //m_saLexoIndex[idx] = read_idx;

	            	//std::cout <<readSeq<<std::endl;
	            	outFile <<readSeq<<'\n';
	                break; // done;
	            }
	        }
	    }

}

void sof::ReadsCollector::build_bwt(std::string readsFileName, std::string bwtFileName) {
	BWTCA::runRopebwt(readsFileName, bwtFileName, false, false);
}
