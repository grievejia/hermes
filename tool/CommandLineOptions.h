#pragma once

#include <experimental/string_view>

class CommandLineOptions
{
private:
	std::experimental::string_view inputFileName;
	std::experimental::string_view outputFileName;
	std::experimental::string_view outputType;
public:
	CommandLineOptions(int argc, char** argv);

	const std::experimental::string_view& getInputFileName() { return inputFileName; }
	const std::experimental::string_view& getOutputFileName() const { return outputFileName; }
	const std::experimental::string_view& getOutputType() const { return outputType; }
};