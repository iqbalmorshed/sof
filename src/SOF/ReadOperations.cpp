/*
 * ReadOperations.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: iqbal
 */

#include "ReadOperations.h"

#include "FileWriter.h"

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

	m_tempEdgeWriter = std::ofstream("temp_edge_file.be");
}

CurrentRead ReadOperations::get_read(numReads_t virtualReadID) {

	return CurrentRead(virtualReadID, m_overlapContainer[virtualReadID],
								m_readsInfo.get_readLen(virtualReadID),
								m_minOverlap);


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

void ReadOperations::write_edges(CurrentRead& currentRead) {

	EdgeInfo edgeInfo;
	edgeInfo.sourceVertexID = currentRead.m_virtualID;

	OverlapInfoVector irreducibleIntervals;

	currentRead.get_all_irreducible_intervals(irreducibleIntervals);

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

}
