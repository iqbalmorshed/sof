/* Bismillahir Rahmanir Rahim
 * FileWriter.h
 *
 *  Created on: Jul 20, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#ifndef SRC_SOF_FILEWRITER_H_
#define SRC_SOF_FILEWRITER_H_

#include <fstream>
#include <vector>

#include "SOFCommon.h"
#include "StringGraph.h"

namespace sof {

struct EdgeInfo {
	numReads_t sourceVertexID;
	std::vector<numReads_t> destinationVertexIDs;
	std::vector<readLen_t> overlapWithSource;

};

class FileWriter {
public:
	FileWriter();
	void write_temporary_edges(	const EdgeInfo& edgeInfo,
								std::ofstream& tempFileWriter);
	void write_asqg_file(	const InputData& inputData,
							const ReadsInfo& readsInfo);
private:
	void write_header(	const InputData &inputData,
						std::ofstream& asqgWriter);
	void write_vertexInfo(	const std::string& readsFileName,
							const ReadsInfo& readsInfo,
							std::vector<std::string>& realIDVector,
							std::ofstream& asqgWriter);
	void write_edgeInfo(const std::vector<std::string>& realIDVector,
						const ReadsInfo& readsInfo,
						std::ofstream& asqgWriter);

	void collect_and_write(	const std::string& temporaryEdgeFileName,
							const std::vector<std::string>& realIDVector,
							const ReadsInfo& readsInfo,
							const EdgeType edgeType,
							std::ofstream& asqgWriter);

	friend class CombinedGraph;
};

} /* namespace sof */

#endif /* SRC_SOF_FILEWRITER_H_ */
