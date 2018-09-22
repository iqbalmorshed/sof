/*
 * RepeatRemoval.cpp
 *
 *  Created on: Sep 17, 2018
 *      Author: iqbal
 */


#include "RepeatRemoval.h"

#include <cstdio>
#include <fstream>
#include <sstream>

#include "Timer.h"
#include "EdgeContainer.h"

namespace sof {

RepeatRemoval::RepeatRemoval(numReads_t numReads) :
								m_numReads(numReads){
	// TODO Auto-generated constructor stub

	Timer t("Repeat removal from temp files");

	remove_repeat("temp_edge_file.bb");
	remove_repeat("temp_edge_file.ee");

}

void RepeatRemoval::remove_repeat(const std::string& tempFileName) {

	std::ifstream inputFile(tempFileName);
	std::ofstream tempFileWriter("temp_file");

	EdgeContainer edgeContainer(m_numReads);

	while (inputFile) {
		// read stuff from the file into a string and print it
		std::string strInput;
		getline(inputFile, strInput);

		std::stringstream ss;
		numReads_t sourceVertex, destinationVertex;
		readLen_t overlapLength;

		ss << strInput;
		ss >> sourceVertex;

		bool isSourceVertexPrinted = false;

		while (ss >> destinationVertex >> overlapLength) {

			if(!edgeContainer.is_present(sourceVertex, destinationVertex, overlapLength)){

				if(!isSourceVertexPrinted){
					tempFileWriter << sourceVertex <<" ";
					isSourceVertexPrinted = true;
				}

				tempFileWriter << destinationVertex <<" "<<overlapLength<<" ";

				edgeContainer.set_edge(sourceVertex, destinationVertex, overlapLength);

			}

		}

		if(isSourceVertexPrinted)
			tempFileWriter<<'\n';

	}

	//rename file
	std::remove(tempFileName.c_str());
	std::rename("temp_file", tempFileName.c_str());
}

} /* namespace sof */
