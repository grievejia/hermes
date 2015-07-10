#include "Util/CommandLine/CommandLineParser.h"

#include <exception>
#include <iomanip>
#include <iostream>

namespace util
{

namespace
{

class ParseException: public std::exception
{
private:
	std::string msg;
public:
	ParseException(const std::string& m): msg(m) {}

	const char* what() const noexcept override
	{
		return msg.data();
	}
};

class HelpException: public std::exception
{
public:
	HelpException() = default;

	const char* what() const noexcept override
	{
		return "-help option is encountered";
	}
};

}

void CommandLineParser::printUsage(const StringView& progName) const
{
	std::cout << "Description: \n  " << helpMsg << "\n\n";

	std::cout << "Usage:\n  " << progName << " [options] ";
	for (auto const& mapping: posFlagMap)
		std::cout << "<" << mapping.first << "> ";
	std::cout << "\n\n";

	std::cout << "Positional arguments:\n";
	for (auto const& mapping: posFlagMap)
	{
		std::cout << "  " << std::left << std::setw(40) << mapping.first << mapping.second.desc << "\n";
	}
	std::cout << "\n";

	std::cout << "Optional arguments:\n";
	for (auto const& mapping: optFlagMap)
	{
		auto argName = "-" + mapping.first.to_string();
		if (mapping.second.defaultValue)
			argName += " <" + mapping.first.to_string() + "_arg>";
		std::cout << "  " << std::left << std::setw(40) << argName << mapping.second.desc;
		if (mapping.second.defaultValue && !(*mapping.second.defaultValue).empty())
			std::cout << " (default = " << *mapping.second.defaultValue<< ")";
		std::cout << "\n";
	}
}

void CommandLineParser::addOptionalFlagEntry(const StringView& name, OptionalFlagEntry&& entry)
{
	auto inserted = optFlagMap.try_emplace(name, std::move(entry)).second;
	assert(inserted && "Duplicate entry in optFlagMap!");
}

void CommandLineParser::addPositionalFlagEntry(const StringView& name, PositionalFlagEntry&& entry)
{
	assert(std::find_if(
		posFlagMap.begin(),
		posFlagMap.end(),
		[&name] (auto const& pair)
		{
			return name == pair.first;
		}
	) == posFlagMap.end() && "Duplicate entry in posFlagMap!");
	
	posFlagMap.emplace_back(std::make_pair(name, std::move(entry)));
}

void CommandLineParser::addPositionalFlag(const StringView& name, const StringView& desc)
{
	PositionalFlagEntry entry = { desc };
	addPositionalFlagEntry(name, std::move(entry));
}

void CommandLineParser::addOptionalFlag(const StringView& name, const StringView& desc)
{
	OptionalFlagEntry entry = { desc, std::experimental::nullopt };
	addOptionalFlagEntry(name, std::move(entry));
}

void CommandLineParser::addOptionalFlag(const StringView& name, const StringView& desc, const StringView& init)
{
	OptionalFlagEntry entry = { desc, init };
	addOptionalFlagEntry(name, std::move(entry));
}

static bool isOption(const std::experimental::string_view& str)
{
	assert(!str.empty());
	return str.front() == '-';
}

void CommandLineParser::parseArgs(const std::vector<StringView>& args, CommandLineFlags& result) const
{
	size_t numPositionalConsumed = 0;
	size_t numFlagConsumed = 0;

	while (numFlagConsumed < args.size())
	{
		auto flag = args[numFlagConsumed];
		if (!isOption(flag))	// Positional arguments
		{
			if (numPositionalConsumed >= posFlagMap.size())
				throw ParseException("too many positional arguments");

			auto inserted = result.addFlag(posFlagMap[numPositionalConsumed].first, flag);
			assert(inserted && "Insertion failed");
			++numPositionalConsumed;
		}
		else	// Optional arguments
		{
			flag.remove_prefix(1);

			// See if flag is "-help"
			if (flag == "help")
				throw HelpException();

			auto itr = optFlagMap.find(flag);
			if (itr == optFlagMap.end())
				throw ParseException("Option not recognized: " + flag.to_string());

			if (itr->second.defaultValue)
			{
				++numFlagConsumed;
				if (numFlagConsumed >= args.size())
					throw ParseException("More argument needs to be provided after option " + flag.to_string());
				auto flagArg = args[numFlagConsumed];
				if (isOption(flagArg))
					throw ParseException("Found another option instead of an argument after option " + flag.to_string());

				auto inserted = result.addFlag(flag, flagArg);
				assert(inserted && "Insertion failed");
			}
			else
			{
				auto inserted = result.addFlag(flag, "");
				assert(inserted && "Insertion failed");
			}
		}
		++numFlagConsumed;
	}

	if (numPositionalConsumed < posFlagMap.size())
		throw ParseException("Not enough positional arguments are provided");

	for (auto const& mapping: optFlagMap)
	{
		if (mapping.second.defaultValue && result.lookup(mapping.first) == nullptr)
			result.addFlag(mapping.first, *mapping.second.defaultValue);
	}
}

CommandLineFlags CommandLineParser::parseCommandLineOptions(int argc, char** argv) const
{
	assert(argc > 0 && "argc cannot be nonpositive!");

	CommandLineFlags result;

	try
	{
		std::vector<StringView> args;
		args.reserve(argc);
		for (auto i = 1; i < argc; ++i)
			args.emplace_back(argv[i]);

		parseArgs(args, result);
	}
	catch (const ParseException& e)
	{
		std::cerr << "Command line options parsing failed: " << e.what() << "\n\n";
		std::cerr << "Please run \"" << argv[0] << " -help\" to view usage information\n";
		std::exit(-1);
	}
	catch (const HelpException& e)
	{
		printUsage(argv[0]);
		std::exit(0);
	}

	return result;
}

}
