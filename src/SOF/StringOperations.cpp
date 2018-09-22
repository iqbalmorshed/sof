/* Bismillahir Rahmanir Rahim
 * StringOperations.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#include "StringOperations.h"

#include <string>
#include <iostream>

namespace sof {

StringOperations::StringOperations() {
	// TODO Auto-generated constructor stub
	set_base_complement_array();
}

//char StringOperations::get_base_complement(char base) {
//
//	if(base == 'A')return 'T';
//	else if(base == 'T')return 'A';
//	else if(base == 'G')return 'C';
//	else if(base == 'C')return 'G';
//	else{
//		std::cout<<"wrong base";
//		return 'X';
//	}
//}

void StringOperations::set_base_complement_array() {
	m_baseComplement['A']='T';
	m_baseComplement['T']='A';
	m_baseComplement['G']='C';
	m_baseComplement['C']='G';
	m_baseComplement['N']='N';
}

void StringOperations::get_reverse_complement(std::string& sequence,
													std::string& reverseComplement) {
	//std::cout<<"in get rc func start: reverseComp"<<reverseComplement<<'\n';
	int len = sequence.length();
	int index2 =0;
	for(int index = len-1; index >=0; index--){
		reverseComplement[index2++]= m_baseComplement[sequence[index]];
	}
	//std::cout<<"in get rc func finish: reverseComp"<<reverseComplement<<'\n';
}

void StringOperations::get_complement(std::string& sequence,
											std::string& complement) {
	int len = sequence.length();
	for(int index =0; index < len; index++){
		complement[index]= m_baseComplement[sequence[index]];
	}

} /* namespace sof */


} /* namespace sof */

