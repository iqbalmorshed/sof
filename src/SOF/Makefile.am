noinst_LIBRARIES = libsof.a

libsof_a_CXXFLAGS = -std=c++11

libsof_a_CPPFLAGS = \
	-I$(top_srcdir)/Util \
	-I$(top_srcdir)/Bigraph \
	-I$(top_srcdir)/SuffixTools \
	-I$(top_srcdir)/StringGraph \
	-I$(top_srcdir)/Concurrency \
	-I$(top_srcdir)/Algorithm \
	-I$(top_srcdir)/SQG \
	-I$(top_srcdir)/Scaffold \
	-I$(top_srcdir)/GraphDiff \
	-I$(top_srcdir)/Thirdparty \
	-I$(top_srcdir)/SGA

libsof_a_SOURCES = \
	sof.h sof.cpp \
	BWT.h BWT.cpp \
	BWTBySampling.h BWTBySampling.cpp \
	CurrentRead.h CurrentRead.cpp \
	EdgeContianer.h EdgeContainer.cpp \
	FileWriter.h FileWriter.cpp \
	GraphFromCombinedEdges.h GraphFromCombinedEdges.cpp \
	LexicographicIndex.h LexicographicIndex.cpp \
	OperationOnBE.h OperationOnBE.cpp \
	OperationOnBBandEE.h OperationOnBBandEE.cpp \
	OverlapOperations.h OverlapOperations.cpp \
	OverlapContainer.h OverlapContainer.cpp	\
	ReadsInfo.h ReadsInfo.cpp \
	ReadOperations.h ReadOperations.cpp \
	ReadsCollector.h ReadsCollector.cpp \
	RepeatRemoval.h RepeatRemoval.cpp \
	SOFCommon.h \
	StringGraph.h StringGraph.cpp \
	StringOperations.h StringOperations.cpp
