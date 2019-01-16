//-----------------------------------------------
// Copyright 2009 Wellcome Trust Sanger Institute
// Written by Jared Simpson (js18@sanger.ac.uk)
// Released under the GPL license
//-----------------------------------------------
//
// index - Build a BWT/FM-index for a set of reads
//
#ifndef INDEXSOF_H
#define INDEXSOF_H
#include <getopt.h>
#include "config.h"
#include "SuffixArray.h"

int indexSOFMain(int argc, char** argv);
//void indexInMemorySAIS();
//void indexInMemoryBCR();
void indexInMemoryRopebwtRC();
//void indexOnDisk();
//void buildIndexForTable(std::string outfile, const ReadTable* pRT, bool isReverse);
void parseIndexSOFOptions(int argc, char** argv);

#endif
