/*
 * OperationOnBE.cpp
 *
 *  Created on: Jun 23, 2018
 *      Author: iqbal
 */

#include "OperationOnBE.h"

#include "ReadOperations.h"
#include "CurrentRead.h"
#include "FileWriter.h"

namespace sof {

OperationOnBE::OperationOnBE(	const BWT* pBWT,
								const OverlapContainer& overlapContainer,
								const LexicographicIndex& lexicoIndex,
								const readLen_t minOverlap,
								ReadsInfo& readsInfo)
		: ReadOperations(pBWT, overlapContainer, lexicoIndex, minOverlap,
							readsInfo) {
	m_tempEdgeWriter = std::ofstream("temp_edge_file.be");
}

CurrentRead OperationOnBE::get_read(numReads_t virtualReadID) {

	return CurrentRead(virtualReadID, m_overlapContainer[virtualReadID],
								m_readsInfo.get_readLen(virtualReadID),
								m_minOverlap);


}

void OperationOnBE::write_edges(CurrentRead& currentRead) {

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

sof::OperationOnBE::~OperationOnBE() {
	//m_tempEdgeWriter.close();
}
