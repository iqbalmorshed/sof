/*
 * ReadOperations.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: iqbal
 */

#include "ReadOperations.h"

namespace sof {

ReadOperations::ReadOperations(	const BWT* pBWT,
								const OverlapContainer& overlapContainer,
								const LexicographicIndex& lexicoIndex,
								const readLen_t minOverlap,
								ReadsInfo& readsInfo)
		: 	m_pBWT(pBWT),
			m_overlapContainer(overlapContainer),
			m_readsInfo(readsInfo),
			m_lexicoIndex(lexicoIndex),
			m_minOverlap(minOverlap){
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
						&& currentRead.m_virtualID != virtualID)

					//if(currentRead.m_virtualID == 12060320)std::cout<<"filter using: virID: "<<virtualID<<",lexID: "<<lexicoID<<" from index: "<<poppedInterval.index<<"\n";
					filter_using_read(currentRead, virtualID,
										poppedInterval.index);
			}
		}
	}
//	std::cout<<"filtering for this read done.\n";
}

void ReadOperations::filter_using_read(	CurrentRead& currentRead,
										numReads_t virtualID,
										readLen_t positionCurrentRead) {


	 auto it = m_overlapContainer[virtualID].end();

	 while( it != m_overlapContainer[virtualID].begin()) {
		 it--;
		 readLen_t splitPosition = it->readIndex + positionCurrentRead;

//		 if(currentRead.m_virtualID == 12060320)std::cout<<"split position: "<<splitPosition<<"curr read max index: "<<currentRead.m_maxIndex<<'\n';

		 if(splitPosition <= currentRead.m_maxIndex){

//			 if(currentRead.m_virtualID == 12060320){
//				 std::cout << "split position: " << int(splitPosition) << "interval: low:"
//								<< it->terminalInterval.lower<<" upper:"<<it->terminalInterval.upper<<'\n';
//			 }
			 currentRead.split_interval(splitPosition, it->terminalInterval);
//			 std::cout<<"splitting done\n";
		 }
		 else
			 break;

	}

}

} /* namespace sof */

sof::ReadOperations::~ReadOperations() {

}
