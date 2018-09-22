/* Bismillahir Rahmanir Rahim
 * test_sof.cpp
 *
 *  Created on: Jul 10, 2018
 *      Author: Iqbal Morshed
 * Institution:	UCF
 */

#include <cstdio>
#include <iostream>
#include <fstream>

#include "BWT.h"
#include "BWTBySampling.h"
#include "BWTByWavelet.h"
#include "CurrentRead.h"
#include "FileWriter.h"
#include "ReadsInfo.h"
#include "StringGraph.h"
#include "LexicographicIndex.h"
#include "OverlapContainer.h"
#include "OperationOnBE.h"
#include "OperationOnBBandEE.h"
#include "ReadOperations.h"
#include "ReadsCollector.h"
#include "Timer.h"

#include <sdsl/wavelet_trees.hpp>
#include <sdsl/suffix_arrays.hpp>



void bwt_test() {

	printf("inside bwt test:\n");
	sof::BWT *pBWT = new sof::BWTBySampling("sample.bwt");
	pBWT->print();
	auto bwtLen = pBWT->get_bwtLength();
	std::cout << "\nbwt length: " << bwtLen << std::endl;

	printf("check interval:\n==========\n");
	sof::BWTInterval bwtInterval;
	sof::TerminalInterval terminalInterval;

	printf("BWT Number of reads:");
	std::cout << pBWT->get_numReads() << '\n';

	while (true) {

		//scanf("%d %d")
		char ch;
		std::cin >> bwtInterval.lower >> bwtInterval.upper >> ch;
		//bwtInterval.lower =
		bwtInterval = pBWT->get_backward_interval(bwtInterval, ch);
		terminalInterval = pBWT->get_backward_terminal_interval(bwtInterval);

		std::cout << "backward interval:" << std::endl;
		std::cout << "lower: " << bwtInterval.lower << " upper: "
				<< bwtInterval.upper << std::endl;

		std::cout << "terminal interval:" << std::endl;
		std::cout << "lower: " << terminalInterval.lower << " upper: "
				<< terminalInterval.upper << std::endl;
	}
	delete pBWT;
}

void lexiocographicIndex_test() {
	sof::LexicographicIndex lexicoIndex("sample.sai");

	printf("convert lexico to virtual ID:\n");
	numReads_t lexicoID;
	while (std::cin >> lexicoID) {
		int x = lexicoIndex[lexicoID];
		std::cout << "lexicoIndex returned:" << x << '\n';
	}
}

void container_test() {

	printf("inside container test:\n");

	sof::BWT *pBWT = new sof::BWTBySampling("sample.bwt");
	sof::LexicographicIndex lexicoIndex("sample.sai");
	//pBWT->print();

	printf("BWT Created.\n");
	sof::ReadsInfo readsInfo;
	readsInfo.set_containers_size(pBWT->get_numReads());
	printf("readsInfo Created\n");
	sof::OverlapContainer overlapContainer("sample.fastq", pBWT, lexicoIndex,
											readsInfo, 2, true);

	printf("Container created.\n");

	printf("print validity info:\n");
	for (auto i = 0; i < readsInfo.get_numReads(); i++) {
		std::cout << i << " " << readsInfo.get_isValid(i) << '\n';
	}
	overlapContainer.print();
}

//void read_collection_from_bwt_test() {
//
//	Timer t("bwt test");
//	sof::ReadsCollector rc("sample.fastq");
//
//	rc.build_bwt("new.bwt");
//
//	rc.collect_read_from_BWT("new.bwt");
//}

//void srr_fastq_to_bwt(std::string fileName){
//	Timer t(fileName+"fastq to bwt converstion");
//
//	sof::ReadsCollector rc(fileName);
//
//	rc.build_bwt(fileName+".bwt");
//}
//
//void srr_bwt_to_readFile(std::string bwtFileName){
//
//	sof::ReadsCollector rc;
//
//	rc.collect_read_from_BWT(bwtFileName);
//}
//
//void srr_fastq_to_readFile(std::string readFileName){
//
//	sof::ReadsCollector rc;
//
//	rc.collect_read_from_fastq(readFileName);
//}

//void test_operationOnBE() {
//	printf("inside test operation on BE:\n");
//
//	sof::BWT *pBWT = new sof::BWTBySampling("sample.bwt");
//	sof::LexicographicIndex lexicoIndex("sample.sai");
//	//pBWT->print();
//
//	printf("BWT Created.\n");
//	sof::ReadsInfo readsInfo;
//	readsInfo.set_containers_size(pBWT->get_numReads());
//	printf("readsInfo Created\n");
//	sof::OverlapContainer overlapContainer("sample.fastq", pBWT, lexicoIndex,
//											readsInfo, 2, true);
//
//	sof::ReadOperations *readOp;
//
//	readOp = new sof::OperationOnBE(nullptr, overlapContainer, lexicoIndex, 2,
//									readsInfo);
//
//	auto numReads = readsInfo.get_numReads();
//
//	for (numReads_t virtualID = 0; virtualID < numReads; virtualID++) {
//
//		if (readsInfo.get_isValid(virtualID)) {
//
//			sof::CurrentRead currentRead = readOp->get_read(virtualID);
//			currentRead.print_intervals();
//			readOp->filter_edges(currentRead);
//			currentRead.print_intervals();
//			readOp->write_edges(currentRead);
//		}
//	}
//
//	std::cout << "job finished" << '\n';
//	delete readOp;
//	std::cout << "really?" << '\n';
//
//	sof::InputData inputData;
//	inputData.readsFileName = "sample.fastq";
//	inputData.minOverlap = 2;
//
//	sof::FileWriter fileWriter;
//	fileWriter.write_asqg_file(inputData, readsInfo);
//
//}
//
//void test_operationOnBB() {
//	printf("inside test operation on BB:\n");
//
//	sof::BWT *pBWT = new sof::BWTBySampling("sample.bwt");
//	sof::LexicographicIndex lexicoIndex("sample.sai");
//	//pBWT->print();
//
//	printf("BWT Created.\n");
//	sof::ReadsInfo readsInfo;
//	readsInfo.set_containers_size(pBWT->get_numReads());
//	printf("readsInfo Created\n");
//	sof::OverlapContainer overlapContainer("sample.fastq", pBWT, lexicoIndex,
//											readsInfo, 2, true);
//
//	sof::ReadOperations *readOp;
//
//	readOp = new sof::OperationOnBBandEE(pBWT, overlapContainer, lexicoIndex, 2,
//											readsInfo, "sample.fastq", BB_EDGE);
//
//	auto numReads = readsInfo.get_numReads();
//
//	for (numReads_t virtualID = 0; virtualID < numReads; virtualID++) {
//
//		if (readsInfo.get_isValid(virtualID)) {
//
//			sof::CurrentRead currentRead = readOp->get_read(virtualID);
//			currentRead.print_intervals();
//			readOp->filter_edges(currentRead);
//			currentRead.print_intervals();
//			readOp->write_edges(currentRead);
//		}
//	}
//
//	std::cout << "job finished" << '\n';
//	delete readOp;
//	std::cout << "really?" << '\n';
//
//	sof::InputData inputData;
//	inputData.readsFileName = "sample.fastq";
//	inputData.minOverlap = 2;
//
//	sof::FileWriter fileWriter;
//	fileWriter.write_asqg_file(inputData, readsInfo);
//
//}

void test_wavelet(){
	sdsl::wt_huff<> wt;
	construct(wt,"input.txt",1);
	for(size_t i=0; i< wt.size() ; i++){
		std::cout<<wt[i]<<" "<<'\n';
	}


}

void test_fm_index(){
	sdsl::csa_wt<> fm_index;
	construct_im(fm_index, "mississippi!", 1);
	std::cout << "'si' occurs " << count(fm_index,"si") << " times.\n";
	store_to_file(fm_index,"fm_index-file.sdsl");
	std::ofstream out("fm_index-file.sdsl.html");
	//write_structure<HTML_FORMAT>(fm_index,out);
}

void test_wavelet_bwt(){

	printf("inside wavelet bwt test:\n");
	sof::BWT *pBWT = new sof::BWTByWavelet("sample.bwt");
	pBWT->print();
	auto bwtLen = pBWT->get_bwtLength();
	std::cout << "\nbwt length: " << bwtLen << std::endl;

	printf("check interval:\n==========\n");
	sof::BWTInterval bwtInterval;
	sof::TerminalInterval terminalInterval;

	printf("BWT Number of reads:");
	std::cout << pBWT->get_numReads() << '\n';

	while (true) {

		//scanf("%d %d")
		char ch;
		std::cin >> bwtInterval.lower >> bwtInterval.upper >> ch;
		//bwtInterval.lower =
		bwtInterval = pBWT->get_backward_interval(bwtInterval, ch);
		terminalInterval = pBWT->get_backward_terminal_interval(bwtInterval);

		std::cout << "backward interval:" << std::endl;
		std::cout << "lower: " << bwtInterval.lower << " upper: "
				<< bwtInterval.upper << std::endl;

		std::cout << "terminal interval:" << std::endl;
		std::cout << "lower: " << terminalInterval.lower << " upper: "
				<< terminalInterval.upper << std::endl;
	}
	delete pBWT;
}
////////////////////////////

void test_sof() {

	//bwt_test();

	//lexiocographicIndex_test();
	//container_test();

	//read_collection_from_bwt_test();

	//srr_fastq_to_bwt("SRR1770413_1.fastq.prep");
	//srr_bwt_to_readFile("SRR1770413_1.fastq.prep.bwt");

	//srr_fastq_to_readFile("SRR1770413_1.fastq.prep");

	//srr_bwt_to_readFile("SRR857279_1.fastq.bwt");

	//srr_fastq_to_readFile("SRR857279_1.fastq.prep");

	//test_operationOnBE();
	//test_operationOnBB();

	//test_wavelet();
	//test_fm_index();
	test_wavelet_bwt();

}

