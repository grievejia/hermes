#include "CommandLineOptions.h"
#include "Parser/TypeBuilder.h"
#include "Printer/ClassPrinter.h"
#include "Printer/SpecPrinter.h"
#include "Printer/UnionPrinter.h"

#include <fstream>
#include <iostream>

using namespace hermes;

void printTypes(const std::experimental::string_view& outputType, const std::experimental::string_view& fileName, const TypeDescriptorList& list)
{
	std::ostream* os = &std::cout;
	std::ofstream fs;
	if (!fileName.empty())
	{
		fs.open(fileName.data());
		if (!fs.is_open())
		{
			std::cerr << "Open output file error\n";
			std::exit(-1);
		}
		os = &fs;
	}

	if (outputType == "text")
		SpecPrinter(*os).printList(list);
	else if (outputType == "class")
		ClassPrinter(*os).printList(list);
	else if (outputType == "union")
		UnionPrinter(*os).printList(list);
	else
	{
		std::cerr << "Output type not recognized: " << outputType << "\n";
		std::exit(-1);
	}
}

int main(int argc, char** argv)
{
	// Unsync iostream with C I/O libraries to accelerate standard iostreams
	std::ios::sync_with_stdio(false);

	CommandLineOptions opts(argc, argv);

	auto typeList = TypeBuilder::loadTypeFromFile(opts.getInputFileName().data());

	printTypes(opts.getOutputType(), opts.getOutputFileName(), typeList);
}