/*
 * OverlapContainer.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: iqbal
 */

#include "OverlapContainer.h"

#include <iostream>

#include "BWT.h"
#include "OverlapOperations.h"
#include "SeqReader.h"
#include "SOFCommon.h"
#include "Timer.h"
#include "Util.h"

namespace sof {

OverlapContainer::OverlapContainer(
									const std::string& readsFileName,
									const BWT* pBWT,
									const LexicographicIndex& lexicoIndex,
									ReadsInfo& readsInfo,
									const readLen_t minOverlap,
									const bool bSetReadsInfo)
		: 	m_readsFileName(readsFileName),
			m_pBWT(pBWT),
			m_lexicoIndex(lexicoIndex),
			m_readsInfo(readsInfo),
			m_minOverlap(minOverlap),
			m_bSetReadsInfo(bSetReadsInfo),
			m_overlapContainerReader(std::ifstream(readsFileName+".container")){



}

const std::vector<OverlapInfo>& OverlapContainer::operator [](const numReads_t index) const {

	assert(index>=0 && index < m_readsInfo.get_numReads());
	return m_container[index];
}

void OverlapContainer::print() {

	for (numReads_t virtualID = 0; virtualID < m_readsInfo.get_numReads();
			virtualID++) {

		std::cout << "readID: " << virtualID;
		for (auto it = m_container[virtualID].begin();
				it != m_container[virtualID].end(); it++) {
			std::cout << " (" << it->terminalInterval.lower << ", "
					<< it->terminalInterval.upper << ") " << int(it->readIndex)
					<< " ] ";
		}
		std::cout << std::endl;
	}
}

Read OverlapContainer::get_read(const SeqRecord& record,
								const numReads_t virtualID) {
	Read read;

	read.virtualID = virtualID;
	read.sequence = record.seq.toString();

//	if(m_containerType == REVERSE_OVERLAPS)
//		std::reverse(read.sequence.begin(), read.sequence.end());

	read.seqLength = record.seq.length();

	return read;
}

void OverlapContainer::update_readsInfo(const Read& read) {

	m_readsInfo.set_readLen(read.virtualID, read.seqLength);
	if(read.seqLength <= m_minOverlap)
		m_readsInfo.set_isValid(read.virtualID, false);

}

void OverlapContainer::update_container(const Read& read, OverlapOperations& overlapOperations) {



	//OverlapInfoVector overlapInfoVector;
	BWTInterval bwtIntervalofIndex1;
	bwtIntervalofIndex1 = overlapOperations.write_overlaps(
			m_pBWT, read.sequence, m_minOverlap, read.virtualID, m_overlapContainerWriter);

	//set reads validity
	if (m_bSetReadsInfo && m_bIsRepeatReadPresent)
		make_repeats_invalid(read.virtualID, read.sequence[0],
								bwtIntervalofIndex1);

}

numReads_t OverlapContainer::get_size() const {
	return m_container.size();
}

void OverlapContainer::writeToFile() {
	Timer t("Overlap Container construction time:");
	m_overlapContainerWriter = std::ofstream(m_readsFileName+".container");
	OverlapOperations overlapOperations;

	SeqReader reader(m_readsFileName, SRF_NO_VALIDATION);
	SeqRecord record;

	//std::cout<<"inside container constructor:"<<std::endl;
	numReads_t virtualID = 0;
	while (reader.get(record)) {

		if (m_readsInfo.get_isValid(virtualID)) {

			Read read = get_read(record, virtualID);
			//if (m_bSetReadsInfo)
			update_readsInfo(read);
			update_container(read, overlapOperations);


			record.seq.reverseComplement();
			read = get_read(record, virtualID+1);
			update_readsInfo(read);
			update_container(read, overlapOperations);

		}
		virtualID+=2;

		if(!(virtualID%20000))
			std::cout<<virtualID<<"number of reads processed\n";

	}
	m_overlapContainerWriter.close();
	std::cout<<"total overlaps written: "<<overlapOperations.getOverlapWriteCount()<<'\n';

}

void OverlapContainer::readFromFile() {

	m_container = std::vector<OverlapInfoVector >(m_readsInfo.get_numReads());

	OverlapOperations overlapOperations;
	overlapOperations.read_overlaps(m_overlapContainerReader, m_container, m_readsInfo);
	std::cout<<"total overlap operations read: "<<overlapOperations.getOverlapReadCount()<<'\n';
}

void OverlapContainer::make_repeats_invalid(numReads_t virtualID,
											char firstBase,
											BWTInterval& bwtIntervalofIndex1) {

	BWTInterval bwtIntervalofIndex0 = m_pBWT->get_backward_interval(
			bwtIntervalofIndex1, firstBase);
	TerminalInterval terminalInterval = m_pBWT->get_backward_terminal_interval(
			bwtIntervalofIndex0);

	if (m_bIsProperSubstringPresent) {
		if (virtualID != m_lexicoIndex[terminalInterval.lower]) {
			m_readsInfo.set_isValid(virtualID, false);
			m_container[virtualID].clear();
		}
	} else {
		assert(virtualID == m_lexicoIndex[terminalInterval.lower]);

		for (auto lexicoID = terminalInterval.lower + 1;
				lexicoID <= terminalInterval.upper; lexicoID++) {

			numReads_t newVirtualID = m_lexicoIndex[lexicoID];
			m_readsInfo.set_isValid(newVirtualID, false);
		}
	}

}

} /* namespace sof */

