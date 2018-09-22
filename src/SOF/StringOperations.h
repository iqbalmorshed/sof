/* Bismillahir Rahmanir Rahim
 * StringOperations.h
 *
 *  Created on: Jul 30, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#ifndef SRC_SOF_STRINGOPERATIONS_H_
#define SRC_SOF_STRINGOPERATIONS_H_

#include <string>

namespace sof {

class StringOperations {
public:
	StringOperations();

	void get_reverse_complement(std::string& sequence, std::string& reverseComplement);
	void get_complement(std::string& sequence, std::string& complement);
	//char get_base_complement(char base);
private:
	char m_baseComplement[128];
	void set_base_complement_array();

};

} /* namespace sof */

#endif /* SRC_SOF_STRINGOPERATIONS_H_ */
