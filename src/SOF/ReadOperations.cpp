/*
 * ReadOperations.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: iqbal
 */

#include "ReadOperations.h"

#include "FileWriter.h"

namespace sof {

ReadOperations::ReadOperations(	const ChunkInfo& chunkInfo,
								const OverlapContainer& overlapContainer,
								const LexicographicIndex& lexicoIndex,
								ReadsInfo& readsInfo,
								const std::string& readsFileName,
								const readLen_t minOverlap)
		: 	m_chunkInfo(chunkInfo),
			m_overlapContainer(overlapContainer),
			m_lexicoIndex(lexicoIndex),
			m_readsInfo(readsInfo),
			m_readsFileName(readsFileName),
			m_minOverlap(minOverlap){


	m_tempEdgeReader = std::ifstream(m_readsFileName + ".filtered-"+ std::to_string(m_chunkInfo.ID));

	if(m_chunkInfo.end == m_readsInfo.get_numReads()-1)
		m_tempEdgeWriter = std::ofstream("temp_edge_file.be");
	else
		m_tempEdgeWriter = std::ofstream(m_readsFileName + ".filtered-"+ std::to_string(m_chunkInfo.ID+1));
	//m_tempEdgeWriter = std::ofstream("temp_edge_file.be");
	//m_tempEdgeWriter = std::ofstream("container-"+(m_chunkInfo.ID+1));
	std::cin.rdbuf(m_tempEdgeReader.rdbuf());

}

//get_read old version
//bool ReadOperations::get_read(CurrentRead& currentRead) {
//
//
//
//	std::string strInput;
//	getline(m_tempEdgeReader, strInput);
//
//	if(strInput!=""){
//		std::stringstream ss;
//		numReads_t virtualReadID;
//		readLen_t seqIndex;
//		TerminalInterval terminalInterval;
//		OverlapInfo overlapInfo;
//		std::vector<OverlapInfo> container;
//
//		ss << strInput;
//		ss >> virtualReadID;
//
////		std::cout<<"inside get read: strInput "<<strInput<<"\n";
//
//		if(m_readsInfo.get_isValid(virtualReadID)){
//			currentRead.m_virtualID = virtualReadID;
//			currentRead.m_maxIndex = m_readsInfo.get_readLen(virtualReadID) - m_minOverlap;
//			currentRead.m_indexedReadOverlaps = std::vector<IntervalList>(currentRead.m_maxIndex + 1);
//			currentRead.m_nElementsInIndex = std::vector<numReads_t>(currentRead.m_maxIndex + 1, 0);
//
//			while (ss >> seqIndex >> terminalInterval.lower >> terminalInterval.upper) {
////				overlapInfo.readIndex = seqIndex;
////				overlapInfo.terminalInterval = terminalInterval;
//				//container.push_back(overlapInfo);
//				//m_overlapReadCount++;
//				currentRead.m_indexedReadOverlaps[seqIndex].push_back(terminalInterval);
//				currentRead.m_nElementsInIndex[seqIndex]++;
//
//				if (currentRead.m_nElementsInIndex[seqIndex] == 1) {
//					currentRead.m_popIndexVector.push_back(seqIndex);
//				}
//
//			}
////			currentRead = CurrentRead(	virtualReadID,
////										container,
////										m_readsInfo.get_readLen(virtualReadID),
////										m_minOverlap);
//		}
//
//
//		return true;
//	}
//
//	return false;
//
//}
bool ReadOperations::get_read(CurrentRead& currentRead) {

		//m_currentRead = currentRead;
		bool newLineFound = 0;
		OverlapInfo overlapInfo;
		m_isValidRead = 1;

		while(!newLineFound){
//			std::cout<<"inside while : chars collected:"<<m_charsCollectedInBuffer<<
//					" totalChars :"<<m_totalCharsInBuffer<<" newLineFound :"<<newLineFound<<'\n';
			 for (int i = m_charsCollectedInBuffer; (i < m_totalCharsInBuffer) && !newLineFound; i++)
			 {

				 //std::cout<<"buffer char in "<<i<<" :"<<m_buffer[i]<<'\n';
				 switch (m_buffer[i])
				 {

					 case '\r':
						 break;
					 case '\n':
						 m_item = 0;
						 m_charsCollectedInBuffer = i+1;
						 m_itemNoInLine = 0;
						 newLineFound = 1;
						 break;
					 case ' ':
						if(m_isValidRead){
							//std::cout<<"in iteration i="<<i<<" :"<<"validity :"<<m_isValidRead<<'\n';
							process_item(currentRead, overlapInfo);
						}
						m_item = 0;
						m_itemNoInLine++;
						break;
					 case '0': case '1': case '2': case '3':
					 case '4': case '5': case '6': case '7':
					 case '8': case '9':
						 m_item = 10*m_item + m_buffer[i] - '0';
						 break;
					 default:
						 std::cerr << "Bad format\n";
				 }
			 }
			 //std::cout<<"for loop passed"<<'\n';
			 if(!newLineFound && std::cin){
				 //std::cout<<"collecting from buffer : buffer size :"<<sizeof(m_buffer)<<"\n";
				 std::cin.read(m_buffer, sizeof(m_buffer));
				 m_totalCharsInBuffer = std::cin.gcount();
				 //std::cout<<"total chars in buffer :"<<m_totalCharsInBuffer<<" total chars Collected"<<m_charsCollectedInBuffer<<"\n";
				 m_charsCollectedInBuffer = 0;

			 }
			 else{
				 //std::cout<<"in else statement\n";

				 break;
			 }
			 //std::cout<<"if-else passed"<<'\n';
		 }

		return newLineFound;



}
void ReadOperations::process_item(	CurrentRead& currentRead,
					OverlapInfo& overlapInfo){


	if(!m_itemNoInLine){
		currentRead.m_virtualID = m_item;

		m_isValidRead = m_readsInfo.get_isValid(currentRead.m_virtualID);

		if(m_isValidRead){
			currentRead.m_maxIndex = m_readsInfo.get_readLen(currentRead.m_virtualID) - m_minOverlap;
			currentRead.m_indexedReadOverlaps = std::vector<IntervalList>(currentRead.m_maxIndex + 1);
			currentRead.m_nElementsInIndex = std::vector<numReads_t>(currentRead.m_maxIndex + 1, 0);
		}
		//std::cout<<"got read :"<<currentRead.m_virtualID<<"\n";
	}
	else{
		//std::cout<<"got item "<<m_itemNoInLine<<" :"<<m_item<<"\n";
		int newItemNo = (m_itemNoInLine-1)%3;
		switch(newItemNo){

			case 0:
				overlapInfo.readIndex = m_item;
				break;
			case 1:
				overlapInfo.terminalInterval.lower = m_item;
				break;
			case 2:
				overlapInfo.terminalInterval.upper = m_item;
				currentRead.m_indexedReadOverlaps[overlapInfo.readIndex].push_back(overlapInfo.terminalInterval);
				currentRead.m_nElementsInIndex[overlapInfo.readIndex]++;

				if (currentRead.m_nElementsInIndex[overlapInfo.readIndex] == 1) {
					currentRead.m_popIndexVector.push_back(overlapInfo.readIndex);
				}
				break;
			default:
				std::cout<<"something wrong...\n";
		}
	}
}
void ReadOperations::filter_edges(CurrentRead& currentRead) {

	while (currentRead.is_poppable_interval()) {
		PoppedInterval poppedInterval = currentRead.pop_intervals_from_lowest_index();


//		if(currentRead.m_virtualID == 12060320){
//			currentRead.print_intervals();
//			std::cout<<"\n popped interval: index: "<<poppedInterval.index<<'\n';
//		}
		for (auto &interval : poppedInterval.intervalVector) {

			for (numReads_t lexicoID = interval.lower;
					lexicoID <= interval.upper; lexicoID++) {

				numReads_t virtualID = m_lexicoIndex[lexicoID];

				//second conditoin in if statement prevents self-edge from filtering
				if (m_readsInfo.get_isValid(virtualID)
						&& currentRead.m_virtualID != virtualID && isWithinRange(virtualID)){

					//if(currentRead.m_virtualID == 12060320)std::cout<<"filter using: virID: "<<virtualID<<",lexID: "<<lexicoID<<" from index: "<<poppedInterval.index<<"\n";
					//puts("before filter_using_read");
					if(m_chunkInfo.start)
						filter_using_read(currentRead, virtualID - m_chunkInfo.start, poppedInterval.index);
					else
						filter_using_read(currentRead, virtualID, poppedInterval.index);
				}

			}
		}
	}
//	std::cout<<"filtering for this read done.\n";
}



void ReadOperations::filter_using_read(	CurrentRead& currentRead,
										numReads_t virtualID,
										readLen_t positionCurrentRead) {


	 for(auto it = m_overlapContainer[virtualID].rbegin();
			 it != m_overlapContainer[virtualID].rend() &&
					 (it->readIndex + positionCurrentRead) <= currentRead.m_maxIndex;
			 it++) {

		 currentRead.split_interval(it->readIndex + positionCurrentRead, it->terminalInterval);
//			 std::cout<<"splitting done\n";
	}

}

void ReadOperations::write_edges(CurrentRead& currentRead) {



	if(m_chunkInfo.end == m_readsInfo.get_numReads()-1)
		write_completely_filtered_edges(currentRead);
	else
		write_partially_filtered_edges(currentRead);



}
void ReadOperations::write_partially_filtered_edges(CurrentRead& currentRead) {

	OverlapInfoVector irreducibleIntervals;

	currentRead.get_all_irreducible_intervals(irreducibleIntervals);

	m_tempEdgeWriter<<currentRead.m_virtualID<<" ";


	for (auto intervalElement = irreducibleIntervals.rbegin();
			intervalElement!= irreducibleIntervals.rend(); intervalElement++) {

		m_tempEdgeWriter<<intervalElement->readIndex<<" "
				<< intervalElement->terminalInterval.lower<<" "
				<< intervalElement->terminalInterval.upper<<" ";
//		std::cout<<"printing: "<<intervalElement->readIndex<<" "
//						<< intervalElement->terminalInterval.lower<<" "
//						<< intervalElement->terminalInterval.upper<<" ";
	}
	m_tempEdgeWriter<<'\n';

}
void ReadOperations::write_completely_filtered_edges(CurrentRead& currentRead) {

	OverlapInfoVector irreducibleIntervals;
	currentRead.get_all_irreducible_intervals(irreducibleIntervals);

	EdgeInfo edgeInfo;
	edgeInfo.sourceVertexID = currentRead.m_virtualID;

	if (irreducibleIntervals.size())
		m_readsInfo.set_isVertex(edgeInfo.sourceVertexID, true);

	bool isSourceRevComp = edgeInfo.sourceVertexID % 2;
	for (auto &intervalElement : irreducibleIntervals) {

		for (auto i = intervalElement.terminalInterval.lower;
				i <= intervalElement.terminalInterval.upper; i++) {

			numReads_t virtualID = m_lexicoIndex[i];

			//prevent invalid reads and self-edges
			if (m_readsInfo.get_isValid(virtualID)
					&& !isBothReverseComplement(isSourceRevComp, virtualID)
					&& virtualID/2 != edgeInfo.sourceVertexID/2) {

				m_readsInfo.set_isVertex(virtualID, true);
				edgeInfo.destinationVertexIDs.push_back(virtualID);
				readLen_t overlapWithSource = m_readsInfo.get_readLen(edgeInfo.sourceVertexID)
						- intervalElement.readIndex;
				edgeInfo.overlapWithSource.push_back(overlapWithSource);
			}

		}
	}

	if(edgeInfo.destinationVertexIDs.size()){
		FileWriter fileWriter;
		fileWriter.write_temporary_edges(edgeInfo, m_tempEdgeWriter);
	}

}



} /* namespace sof */

sof::ReadOperations::~ReadOperations() {
	m_tempEdgeReader.close();
	m_tempEdgeWriter.close();
	if(m_chunkInfo.ID ){
		std::string fileName = m_readsFileName + ".filtered-"+ std::to_string(m_chunkInfo.ID);
		remove(fileName.c_str());
	}
}
