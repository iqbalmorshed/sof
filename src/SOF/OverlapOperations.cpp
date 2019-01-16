/* Bismillahir Rahmanir Rahim
 * OverlapOperations.cpp
 *
 *  Created on: Jul 22, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#include "OverlapOperations.h"


#include <iostream>
#include <string>


namespace sof {

OverlapOperations::OverlapOperations() {
	// TODO Auto-generated constructor stub

}

BWTInterval OverlapOperations::write_overlaps(const BWT* pBWT,
											const std::string& sequence,
											const readLen_t minOverlap,
											const numReads_t virtualID,
											std::ofstream& overlapWriter) {
	readLen_t seqLength = sequence.length();
	BWTInterval bwtInterval;
	TerminalInterval terminalInterval;
	//OverlapInfo overlapInfo;

	bwtInterval.lower = 0;
	bwtInterval.upper = pBWT->get_bwtLength() - 1;

	int writeCount = 0;
	//std::cout<<virtualID<<" ";
	overlapWriter<<virtualID<<" ";
	for (readLen_t seqIndex = seqLength - 1; seqIndex >= 1; seqIndex--) {

		char base = sequence[seqIndex];
		bwtInterval = pBWT->get_backward_interval(bwtInterval, base);
		//terminalInterval = pBWT->get_backward_terminal_interval(bwtInterval);

		if (seqIndex <= seqLength - minOverlap) {

			terminalInterval = pBWT->get_backward_terminal_interval(bwtInterval);

			if(terminalInterval.lower <= terminalInterval.upper){
				//overlapInfo.terminalInterval = terminalInterval;
				//overlapInfo.readIndex = seqIndex;

				//overlapInfoVector.push_back(overlapInfo);
				//std::cout<<seqIndex<<" "<<terminalInterval.lower<<" "<<terminalInterval.upper<<" ";
				overlapWriter<<seqIndex<<" "<<terminalInterval.lower<<" "<<terminalInterval.upper<<" ";
				m_overlapWriteCount++;
			}
		}
	}
	overlapWriter<<'\n';

	return bwtInterval;
}

BWTInterval OverlapOperations::get_overlaps(const BWT* pBWT,
											const std::string& sequence,
											const readLen_t minOverlap,
											OverlapInfoVector& overlapInfoVector) {
	readLen_t seqLength = sequence.length();
	BWTInterval bwtInterval;
	TerminalInterval terminalInterval;
	OverlapInfo overlapInfo;

	bwtInterval.lower = 0;
	bwtInterval.upper = pBWT->get_bwtLength() - 1;

	for (readLen_t seqIndex = seqLength - 1; seqIndex >= 1; seqIndex--) {

		char base = sequence[seqIndex];
		bwtInterval = pBWT->get_backward_interval(bwtInterval, base);
		//terminalInterval = pBWT->get_backward_terminal_interval(bwtInterval);

		if (seqIndex <= seqLength - minOverlap) {

			terminalInterval = pBWT->get_backward_terminal_interval(bwtInterval);

			if(terminalInterval.lower <= terminalInterval.upper){
				overlapInfo.terminalInterval = terminalInterval;
				overlapInfo.readIndex = seqIndex;

				overlapInfoVector.push_back(overlapInfo);
			}
		}
	}
	return bwtInterval;
}

void sof::OverlapOperations::read_overlaps(	std::ifstream& overlapReader,
											std::vector<OverlapInfoVector>& container,
											const ReadsInfo& readsInfo) {

	//std::cout<<"inside read_overlaps"<<'\n';
	while (overlapReader) {
		// read stuff from the file into a string and print it
		//std::cout<<"inside read_overlaps: while loop started"<<'\n';
		std::string strInput;
		getline(overlapReader, strInput);

		std::stringstream ss;
		numReads_t sourceVertex;
		readLen_t seqIndex;
		TerminalInterval terminalInterval;
		OverlapInfo overlapInfo;

		//strInput.erase(strInput.find_last_not_of(" \t\n\r\f\v") + 1);

		ss << strInput;
		ss >> sourceVertex;
		//std::cout<<strInput<<'\n';
		//std::cout<<sourceVertex<<" ";
		//std::cout<<"inside read_overlaps: before if "<<sourceVertex<<'\n';
		if(readsInfo.get_isValid(sourceVertex)){
			//std::cout<<"inside read_overlaps: after if"<<'\n';
			while (ss >> seqIndex >> terminalInterval.lower >> terminalInterval.upper) {
				//std::cout<<seqIndex <<" "<< terminalInterval.lower <<" "<< terminalInterval.upper<<" ";
				overlapInfo.readIndex = seqIndex;
				overlapInfo.terminalInterval = terminalInterval;
				container[sourceVertex].push_back(overlapInfo);
				m_overlapReadCount++;
			}
		}
//		if(readsInfo.get_numReads()/4 < sourceVertex)
//			exit(0);
		//std::cout<<"\n";
		//std::cout<<"inside read_overlaps: while loop ended"<<'\n';
	}


}

}

