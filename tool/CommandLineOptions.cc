#include "CommandLineOptions.h"
#include "Util/CommandLine/TypedCommandLineParser.h"

using namespace util;

CommandLineOptions::CommandLineOptions(int argc, char** argv): outputFileName("")
{
	TypedCommandLineParser cmdParser("Hermes the Algebraic Data Type tool for C++14");
	cmdParser.addStringPositionalFlag("inputFile", "Input LLVM bitcode file name", inputFileName);
	cmdParser.addStringPositionalFlag("outputType", "Choose the form of output (text, class, union)", outputType);
	cmdParser.addStringOptionalFlag("o", "Output LLVM bitcode file name (default to stdout)", outputFileName);

	cmdParser.parseCommandLineOptions(argc, argv);
}