/* Bismillahir Rahmanir Rahim
 * FileWriter.cpp
 *
 *  Created on: Jul 20, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#include "FileWriter.h"

#include <sstream>
#include <string>

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


#include "ASQG.h"
#include "SeqReader.h"
#include "ReadsInfo.h"

namespace sof {

FileWriter::FileWriter() {
	// TODO Auto-generated constructor stub

}

void FileWriter::write_asqg_file(	const InputData& inputData,
									const ReadsInfo& readsInfo) {

	std::ofstream asqgWriter(inputData.readsFileName + ".asqg");
	std::vector<std::string> realIDVector(readsInfo.get_numReads()/2, "");

	write_header(inputData, asqgWriter);
	write_vertexInfo(inputData.readsFileName, readsInfo, realIDVector,
						asqgWriter);
	write_edgeInfo(realIDVector, readsInfo, asqgWriter);
}

void FileWriter::write_temporary_edges(const EdgeInfo& edgeInfo,
											std::ofstream& tempFileWriter) {
//	std::cout<<"in write_temporary_edge function:\n";

	if(!tempFileWriter)
		std::cout<<"can't opent temp file.";

	tempFileWriter << edgeInfo.sourceVertexID << " ";

	int len = edgeInfo.destinationVertexIDs.size();
	for (numReads_t i = 0; i < len; i++) {
		tempFileWriter << edgeInfo.destinationVertexIDs[i] << " "
				<< int(edgeInfo.overlapWithSource[i]) << " ";
	}
	tempFileWriter << '\n';
}

void FileWriter::write_header(	const InputData &inputData,
					std::ofstream& asqgWriter) {

	ASQG::HeaderRecord headerRecord;
	headerRecord.setOverlapTag(inputData.minOverlap);
	headerRecord.setErrorRateTag(inputData.errorRate);
	headerRecord.setInputFileTag(inputData.readsFileName);
	headerRecord.setContainmentTag(true); // containments are always present
	headerRecord.setTransitiveTag(!inputData.bIrreducibleOnly);
	headerRecord.write(asqgWriter);
}

void FileWriter::write_vertexInfo(	const std::string& readsFileName,
						const ReadsInfo& readsInfo,
						std::vector<std::string>& realIDVector,
						std::ofstream& asqgWriter) {

	SeqReader reader(readsFileName, SRF_NO_VALIDATION);
	SeqRecord record;

	numReads_t virtualID = 0;

	while (reader.get(record)) {

		if (readsInfo.get_isVertex(virtualID)) {
			std::string readID = record.id;
			std::string sequence = record.seq.toString();
			realIDVector[virtualID/2] = readID;

			asqgWriter << "VT\t" << readID << "\t" << sequence << '\n';
		}
		virtualID+=2;

	}
//	std::cout<<"vertexinfo writing finished...\n";
//	std::this_thread::sleep_for (std::chrono::seconds(10));


}

void FileWriter::write_edgeInfo(const std::vector<std::string>& realIDVector,
					const ReadsInfo& readsInfo,
					std::ofstream& asqgWriter) {

//	collect_and_write("temp_edge_file.be", realIDVector, readsInfo, BE_EDGE, asqgWriter);

	std::ifstream inputFile("temp_edge_file.be");

	while (inputFile) {
		// read stuff from the file into a string and print it
		std::string strInput;
		getline(inputFile, strInput);

		std::stringstream ss;
		numReads_t sourceVertex;
		numReads_t destinationVertex;
		readLen_t overlapLength;
		readLen_t sourceIndex;

		//strInput.erase(strInput.find_last_not_of(" \t\n\r\f\v") + 1);

		ss << strInput;
		ss >> sourceVertex;

		bool isSourceRC = sourceVertex%2;
		while (ss >> destinationVertex >> overlapLength) {

			bool isDestinationRC = destinationVertex%2;
			asqgWriter <<"ED\t"<< realIDVector[sourceVertex/2] << " "
					<< realIDVector[destinationVertex/2] << " ";

			sourceIndex = readsInfo.get_readLen(sourceVertex) - overlapLength;
			if(!isSourceRC && !isDestinationRC){ //BE edge

				int sourceStartIndex = sourceIndex;
				int sourceLength = readsInfo.get_readLen(sourceVertex);
				int sourceEndIndex = sourceLength -1;

				int destinationStartIndex = 0;
				int destinationEndIndex = sourceEndIndex -sourceStartIndex;
				int destinationLength = readsInfo.get_readLen(destinationVertex);

				asqgWriter << sourceStartIndex << " "
						<< sourceEndIndex << " "
						<< sourceLength << " "
						<< destinationStartIndex<< " "
						<< destinationEndIndex<< " "
						<< destinationLength<< " "
						<< 0 << " " << 0 << '\n';
				//
			}
			else if(isSourceRC){ //BB EDGE

				int sourceStartIndex = 0;
				int sourceLength = readsInfo.get_readLen(sourceVertex);
				int sourceEndIndex = sourceLength - 1 - sourceIndex;

				int destinationStartIndex = 0;
				int destinationEndIndex = sourceEndIndex;
				int destinationLength = readsInfo.get_readLen(destinationVertex);

				asqgWriter << sourceStartIndex << " "
						<< sourceEndIndex << " "
						<< sourceLength << " "
						<< destinationStartIndex<< " "
						<< destinationEndIndex<< " "
						<< destinationLength<< " "
						<< 1 << " " << 0 << '\n';
			}
			else{ //EE Edge

				int sourceStartIndex = sourceIndex;
				int sourceEndIndex = readsInfo.get_readLen(sourceVertex) -1;
				int sourceLength = readsInfo.get_readLen(sourceVertex);

				int destinationLength = readsInfo.get_readLen(destinationVertex);
				int destinationEndIndex = destinationLength - 1;
				int destinationStartIndex = destinationEndIndex - (sourceEndIndex - sourceStartIndex);



				asqgWriter << sourceStartIndex << " "
						<< sourceEndIndex << " "
						<< sourceLength << " "
						<< destinationStartIndex<< " "
						<< destinationEndIndex<< " "
						<< destinationLength<< " "
						<< 1 << " " << 0 << '\n';

			}
		}
	}


	//std::remove("temp_edge_file.be");


}



//void FileWriter::write_edgeInfo(const std::vector<std::string>& realIDVector,
//					const ReadsInfo& readsInfo,
//					std::ofstream& asqgWriter) {
//
//	collect_and_write("temp_edge_file.be", realIDVector, readsInfo, BE_EDGE, asqgWriter);
//	collect_and_write("temp_edge_file.bb", realIDVector, readsInfo, BB_EDGE, asqgWriter);
//	collect_and_write("temp_edge_file.ee", realIDVector, readsInfo, EE_EDGE, asqgWriter);
//
//	std::remove("temp_edge_file.be");
//	std::remove("temp_edge_file.bb");
//	std::remove("temp_edge_file.ee");
//
//
//}
//
//void FileWriter::collect_and_write(	const std::string& temporaryEdgeFileName,
//									const std::vector<std::string>& realIDVector,
//									const ReadsInfo& readsInfo,
//									const EdgeType edgeType,
//									std::ofstream& asqgWriter) {
//
//	std::ifstream inputFile(temporaryEdgeFileName);
//
//	while (inputFile) {
//		// read stuff from the file into a string and print it
//		std::string strInput;
//		getline(inputFile, strInput);
//
//		std::stringstream ss;
//		numReads_t sourceVertex;
//		numReads_t destinationVertex;
//		readLen_t overlapLength;
//		readLen_t sourceIndex;
//
//		//strInput.erase(strInput.find_last_not_of(" \t\n\r\f\v") + 1);
//
//		ss << strInput;
//		ss >> sourceVertex;
//
//
//		while (ss >> destinationVertex >> overlapLength) {
//
//			asqgWriter <<"ED\t"<< realIDVector[sourceVertex] << " "
//					<< realIDVector[destinationVertex] << " ";
//
//			sourceIndex = readsInfo.get_readLen(sourceVertex) - overlapLength;
//			if (edgeType == BE_EDGE) {
//
//				int sourceStartIndex = sourceIndex;
//				int sourceLength = readsInfo.get_readLen(sourceVertex);
//				int sourceEndIndex = sourceLength -1;
//
//				int destinationStartIndex = 0;
//				int destinationEndIndex = sourceEndIndex -sourceStartIndex;
//				int destinationLength = readsInfo.get_readLen(destinationVertex);
//
//				asqgWriter << sourceStartIndex << " "
//						<< sourceEndIndex << " "
//						<< sourceLength << " "
//						<< destinationStartIndex<< " "
//						<< destinationEndIndex<< " "
//						<< destinationLength<< " "
//						<< 0 << " " << 0 << '\n';
//			}
//			else if(edgeType == BB_EDGE){
//
//					int sourceStartIndex = 0;
//					int sourceLength = readsInfo.get_readLen(sourceVertex);
//					int sourceEndIndex = sourceLength - 1 - sourceIndex;
//
//					int destinationStartIndex = 0;
//					int destinationEndIndex = sourceEndIndex;
//					int destinationLength = readsInfo.get_readLen(destinationVertex);
//
//					asqgWriter << sourceStartIndex << " "
//							<< sourceEndIndex << " "
//							<< sourceLength << " "
//							<< destinationStartIndex<< " "
//							<< destinationEndIndex<< " "
//							<< destinationLength<< " "
//							<< 1 << " " << 0 << '\n';
//			}
//			else{ //EE_EDGE
//
//				int sourceStartIndex = sourceIndex;
//				int sourceEndIndex = readsInfo.get_readLen(sourceVertex) -1;
//				int sourceLength = readsInfo.get_readLen(sourceVertex);
//
//				int destinationLength = readsInfo.get_readLen(destinationVertex);
//				int destinationEndIndex = destinationLength - 1;
//				int destinationStartIndex = destinationEndIndex - (sourceEndIndex - sourceStartIndex);
//
//
//
//				asqgWriter << sourceStartIndex << " "
//						<< sourceEndIndex << " "
//						<< sourceLength << " "
//						<< destinationStartIndex<< " "
//						<< destinationEndIndex<< " "
//						<< destinationLength<< " "
//						<< 1 << " " << 0 << '\n';
//
//			}
//
//		}
//
//	}
//
//}

} /* namespace sof */
