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

//numReads_t sof::OverlapOperations::read_overlaps_upto_limit(
//		std::ifstream& overlapReader,
//		std::vector<OverlapInfoVector>& container,
//		const ReadsInfo& readsInfo,
//		int maxContainerSize) {
//
//	m_overlapReadCount =0;
//	numReads_t sourceVertex;
//
//	numReads_t readCount = 0;
//	std::cout<<"maxContainer size: "<<maxContainerSize<<'\n';
////	std::cout<<"overlapReader Is empty?"<<overlapReader<<'\n';
//	while (overlapReader && m_overlapReadCount < maxContainerSize) {
//		std::string strInput;
//		getline(overlapReader, strInput);
//
//		std::stringstream ss;
//
//		readLen_t seqIndex;
//		TerminalInterval terminalInterval;
//		OverlapInfo overlapInfo;
//
//		ss << strInput;
//		ss >> sourceVertex;
//
//		container.push_back(std::vector<OverlapInfo>());
//
//		if(readsInfo.get_isValid(sourceVertex)){
//
//			while (ss >> seqIndex >> terminalInterval.lower >> terminalInterval.upper) {
//				overlapInfo.readIndex = seqIndex;
//				overlapInfo.terminalInterval = terminalInterval;
//				container[readCount].push_back(overlapInfo);
//				m_overlapReadCount++;
//			}
//		}
//		readCount++;
//	}
//	std::cout<<"total number of overlaps in this chunk: "<<m_overlapReadCount<<'\n';
//	std::cout<<"overlapInfo size :"<<sizeof(OverlapInfo)<<'\n';
//	std::cout<<"total size in bytes: "<<m_overlapReadCount*sizeof(OverlapInfo)<<'\n';
//	std::cout<<"container capacity: "<<container.capacity()<<'\n';
//	std::cout<<"container size: "<<container.size()<<'\n';
//	return sourceVertex;
//
//}

numReads_t sof::OverlapOperations::read_overlaps_upto_limit(
		std::ifstream& overlapReader,
		std::vector<OverlapInfoVector>& container,
		const ReadsInfo& readsInfo,
		int maxContainerSize,
		BufferInfo& bi) {

	m_overlapReadCount =0;
	//numReads_t sourceVertex;

	m_readCount = 0;
	std::cout<<"maxContainer size: "<<maxContainerSize<<'\n';
	std::cin.rdbuf(overlapReader.rdbuf());

	OverlapInfo overlapInfo;


	bool newLineFound = 0;
	while(m_overlapReadCount < maxContainerSize || !newLineFound){
//			std::cout<<"inside while : chars collected:"<<bi.charsCollectedInBuffer<<
//					" totalChars :"<<bi.totalCharsInBuffer<<'\n';
		m_isValidRead = 1;
		newLineFound = 0;
		int i;
		for (i = bi.charsCollectedInBuffer; (i < bi.totalCharsInBuffer) && !newLineFound; i++){

			 switch (bi.buffer[i])
			 {
				 case '\r':
					 break;
				 case '\n':
					 bi.item = 0;
					 bi.charsCollectedInBuffer = i+1;
					 bi.itemNoInLine = 0;
					 newLineFound = 1;
					 m_readCount++;
					 break;
				 case ' ':
					if(m_isValidRead){
						//std::cout<<"in iteration i="<<i<<" :"<<"validity :"<<m_isValidRead<<'\n';
						process_item(container, overlapInfo, readsInfo,
								bi.item, bi.itemNoInLine);

					}
					bi.item = 0;
					bi.itemNoInLine++;
					break;
				 case '0': case '1': case '2': case '3':
				 case '4': case '5': case '6': case '7':
				 case '8': case '9':
					 bi.item = 10*bi.item + bi.buffer[i] - '0';
					 break;
				 default:
					 std::cerr << "Bad format\n";
			 }
		 }
		 //std::cout<<"for loop passed"<<'\n';
		 if(!newLineFound && std::cin){
			 std::cin.read(bi.buffer, sizeof(bi.buffer));
			 bi.totalCharsInBuffer = std::cin.gcount();
			 bi.charsCollectedInBuffer = 0;
		 }

		 if(!std::cin && i == bi.totalCharsInBuffer)
			 break;

	 }

	std::cout<<"total number of overlaps in this chunk: "<<m_overlapReadCount<<'\n';
	std::cout<<"overlapInfo size :"<<sizeof(OverlapInfo)<<'\n';
	std::cout<<"total size in bytes: "<<m_overlapReadCount*sizeof(OverlapInfo)<<'\n';
	std::cout<<"container capacity: "<<container.capacity()<<'\n';
	std::cout<<"container size: "<<container.size()<<'\n';
	return m_sourceVertex;

}
void sof::OverlapOperations::process_item(	std::vector<OverlapInfoVector>& container,
											OverlapInfo& overlapInfo,
											const ReadsInfo& readsInfo,
											const numReads_t& item,
											const int& itemNoInLine){


	if(!itemNoInLine){
		m_sourceVertex = item;
		m_isValidRead = readsInfo.get_isValid(m_sourceVertex);
		//std::cout<<"got read :"<<currentRead.m_virtualID<<"\n";
		container.push_back(std::vector<OverlapInfo>());
	}
	else{
		//std::cout<<"got item "<<m_itemNoInLine<<" :"<<m_item<<"\n";
		int newItemNo = (itemNoInLine-1)%3;
		switch(newItemNo){

			case 0:
				overlapInfo.readIndex = item;
				break;
			case 1:
				overlapInfo.terminalInterval.lower = item;
				break;
			case 2:
				overlapInfo.terminalInterval.upper = item;
				container[m_readCount].push_back(overlapInfo);
				m_overlapReadCount++;
				break;
			default:
				std::cout<<"something wrong...\n";
		}
	}
}
