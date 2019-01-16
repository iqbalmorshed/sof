/*
 * sof.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: iqbal
 */


#include "sof.h"

#define NDEBUG

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <assert.h>

#include "Util.h"
#include "overlap.h"
#include "SuffixArray.h"
#include "BWT.h"
#include "SGACommon.h"
#include "OverlapCommon.h"
#include "Timer.h"
#include "BWTAlgorithms.h"
#include "ASQG.h"
#include "gzstream.h"
#include "SequenceProcessFramework.h"
#include "OverlapProcess.h"
#include "ReadInfoTable.h"

#include "SOFCommon.h"
#include "StringGraph.h"
//#include "test_sof.cpp"



enum OutputType
{
    OT_ASQG,
    OT_RAW
};

#define SUBPROGRAM "sof"
static const char *OVERLAP_VERSION_MESSAGE =
SUBPROGRAM " Version " PACKAGE_VERSION "\n"
"Written by S M Iqbal Morshed.\n"
"\n";

static const char *OVERLAP_USAGE_MESSAGE =
"Usage: " PACKAGE_NAME " " SUBPROGRAM " [OPTION] ... READSFILE\n"
"Compute string graph using SOF algorithm\n"
"\n"
"      --help                           display this help and exit\n"
"      -m, --min-overlap=LEN            minimum overlap required between two reads (default: 45)\n"
"      -p, --prefix=PREFIX              use PREFIX for the names of the index files (default: prefix of the input file)\n"
"      -f, --target-file=FILE           perform the overlap queries against the reads in FILE\n"
"\nReport bugs to imorshed@knights.ucf.edu \n\n";

static const char* PROGRAM_IDENT =
PACKAGE_NAME "::" SUBPROGRAM;

namespace opt
{
    static unsigned int verbose;
    static int numThreads = 1;
    static OutputType outputType = OT_ASQG;
    static std::string readsFile;
    static std::string targetFile;
    static std::string outFile;
    static std::string prefix;

    static double errorRate = 0.0f;
    static unsigned int minOverlap = DEFAULT_MIN_OVERLAP;
    static int seedLength = 0;
    static int seedStride = 0;
    static int sampleRate = BWT::DEFAULT_SAMPLE_RATE_SMALL;
    static bool bIrreducibleOnly = true;
    static bool bExactIrreducible = false;
}

static const char* shortopts = "m:d:e:t:l:s:o:f:p:vix";

enum { OPT_HELP = 1, OPT_VERSION, OPT_EXACT };

static const struct option longopts[] = {
    { "verbose",     no_argument,       NULL, 'v' },
    { "threads",     required_argument, NULL, 't' },
    { "min-overlap", required_argument, NULL, 'm' },
    { "sample-rate", required_argument, NULL, 'd' },
    { "outfile",     required_argument, NULL, 'o' },
    { "target-file", required_argument, NULL, 'f' },
    { "prefix",      required_argument, NULL, 'p' },
    { "error-rate",  required_argument, NULL, 'e' },
    { "seed-length", required_argument, NULL, 'l' },
    { "seed-stride", required_argument, NULL, 's' },
    { "exhaustive",  no_argument,       NULL, 'x' },
    { "exact",       no_argument,       NULL, OPT_EXACT },
    { "help",        no_argument,       NULL, OPT_HELP },
    { "version",     no_argument,       NULL, OPT_VERSION },
    { NULL, 0, NULL, 0 }
};

//
// Main
//
int sofMain(int argc, char** argv){

	Timer t("SOF: String graph construction time:");
	parseSOFOptions(argc, argv);

	// Determine which index files to use. If a target file was provided,
	// use the index of the target reads
	std::string indexPrefix;
	if(!opt::prefix.empty())
	  indexPrefix = opt::prefix;
	else
	{
	  if(!opt::targetFile.empty())
		indexPrefix = stripExtension(opt::targetFile);
	  else
		indexPrefix = stripExtension(opt::readsFile);
	}


	sof::InputData inputData;

	inputData.readsFileName = opt::readsFile;
	inputData.bwtFileName = indexPrefix + BWTRC_EXT;
	//inputData.revBwtFileName = indexPrefix + RBWT_EXT;
	inputData.lexIndexFileName = indexPrefix + SAIRC_EXT;
	//inputData.revLexIndexFileName = indexPrefix + RSAI_EXT;
	inputData.minOverlap = opt::minOverlap;

	std::cout<<"input Data Collected.\n";
	sof::StringGraph stringGraph(inputData);
	stringGraph.construct();

	//test_sof();



	return 0;
}

void parseSOFOptions(int argc, char** argv)
{
    bool die = false;
    for (char c; (c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1;)
    {
        std::istringstream arg(optarg != NULL ? optarg : "");
        switch (c)
        {
            case 'm': arg >> opt::minOverlap; break;
            case 'o': arg >> opt::outFile; break;
            case 'p': arg >> opt::prefix; break;
            case 'e': arg >> opt::errorRate; break;
            case 't': arg >> opt::numThreads; break;
            case 'l': arg >> opt::seedLength; break;
            case 's': arg >> opt::seedStride; break;
            case 'd': arg >> opt::sampleRate; break;
            case 'f': arg >> opt::targetFile; break;
            case OPT_EXACT: opt::bExactIrreducible = true; break;
            case 'x': opt::bIrreducibleOnly = false; break;
            case '?': die = true; break;
            case 'v': opt::verbose++; break;
            case OPT_HELP:
                std::cout << OVERLAP_USAGE_MESSAGE;
                exit(EXIT_SUCCESS);
            case OPT_VERSION:
                std::cout << OVERLAP_VERSION_MESSAGE;
                exit(EXIT_SUCCESS);
        }
    }

    if (argc - optind < 1)
    {
        std::cerr << SUBPROGRAM ": missing arguments\n";
        die = true;
    }
    else if (argc - optind > 1)
    {
        std::cerr << SUBPROGRAM ": too many arguments\n";
        die = true;
    }

    if(opt::numThreads <= 0)
    {
        std::cerr << SUBPROGRAM ": invalid number of threads: " << opt::numThreads << "\n";
        die = true;
    }

    if(!IS_POWER_OF_2(opt::sampleRate))
    {
        std::cerr << SUBPROGRAM ": invalid parameter to -d/--sample-rate, must be power of 2. got: " << opt::sampleRate << "\n";
        die = true;
    }

    if (die)
    {
        std::cout << "\n" << OVERLAP_USAGE_MESSAGE;
        exit(EXIT_FAILURE);
    }

    // Validate parameters
    if(opt::errorRate <= 0)
        opt::errorRate = 0.0f;

    if(opt::seedLength < 0)
        opt::seedLength = 0;

    if(opt::seedLength > 0 && opt::seedStride <= 0)
        opt::seedStride = opt::seedLength;

    // Parse the input filenames
    opt::readsFile = argv[optind++];

    if(opt::outFile.empty())
    {
        std::string prefix = stripFilename(opt::readsFile);
        if(!opt::targetFile.empty())
        {
            prefix.append(1,'.');
            prefix.append(stripFilename(opt::targetFile));
        }
        opt::outFile = prefix + ASQG_EXT + GZIP_EXT;
    }
}

