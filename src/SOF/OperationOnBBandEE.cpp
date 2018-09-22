/* Bismillahir Rahmanir Rahim
 * OperationOnBB.cpp
 *
 *  Created on: Jul 22, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#include "OperationOnBBandEE.h"

#include "FileWriter.h"
#include "OverlapOperations.h"
#include "StringOperations.h"

namespace sof {

OperationOnBBandEE::OperationOnBBandEE(	const BWT* pBWT,
										const OverlapContainer& overlapContainer,
										const LexicographicIndex& lexicoIndex,
										const readLen_t minOverlap,
										ReadsInfo& readsInfo,
										const std::string& readsFileName,
										const EdgeType edgeType)
		: 	ReadOperations(pBWT, overlapContainer, lexicoIndex, minOverlap,
							readsInfo),
			m_readsFileName(readsFileName),
			m_seqReader(m_readsFileName, SRF_NO_VALIDATION),
			m_edgeType(edgeType) {

	if(edgeType==BB_EDGE)
		m_tempEdgeWriter = std::ofstream("temp_edge_file.bb");
	else
		m_tempEdgeWriter = std::ofstream("temp_edge_file.ee");
}

CurrentRead OperationOnBBandEE::get_read(numReads_t virtualID) {

	SeqRecord record;
	OverlapOperations overlapOp;

	while (m_nReadsCollected <= virtualID) {
		m_seqReader.get(record);
		m_nReadsCollected++;
	}

	std::string readSeq = record.seq.toString();
	std::string newReadSeq(readSeq.length(),'A');

	//std::cout<<"readSeq"<<readSeq<<'\n';
	if (m_edgeType == BB_EDGE) {
		m_stringOperations.get_reverse_complement(readSeq, newReadSeq);
	} else if (m_edgeType == EE_EDGE) {
		m_stringOperations.get_complement(readSeq, newReadSeq);
	}
	//std::cout<<"newReadSeq"<<newReadSeq<<"\n\n";
	OverlapInfoVector readOverlaps;
	overlapOp.get_overlaps(m_pBWT, newReadSeq, m_minOverlap, readOverlaps);

	return CurrentRead(virtualID, readOverlaps, readSeq.length(), m_minOverlap);

}


void OperationOnBBandEE::write_edges(CurrentRead& currentRead) {

	EdgeInfo edgeInfo;
	edgeInfo.sourceVertexID = currentRead.m_virtualID;

	OverlapInfoVector irreducibleIntervals;

	currentRead.get_all_irreducible_intervals(irreducibleIntervals);

	if (irreducibleIntervals.size())
		m_readsInfo.set_isVertex(edgeInfo.sourceVertexID, true);

	for (auto &intervalElement : irreducibleIntervals) {

		for (auto i = intervalElement.terminalInterval.lower;
				i <= intervalElement.terminalInterval.upper; i++) {

			numReads_t virtualID = m_lexicoIndex[i];

			//prevent invalid reads and self-edges and prevent repeat edges
			if (m_readsInfo.get_isValid(virtualID)
					&& virtualID != edgeInfo.sourceVertexID
					/*&& virtualID > edgeInfo.sourceVertexID*/) {

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

OperationOnBBandEE::~OperationOnBBandEE() {
}


} /* namespace sof */

