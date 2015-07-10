#pragma once

#include "Util/CommandLine/CommandLineFlags.h"

#include <experimental/optional>

namespace util
{

class CommandLineParser
{
private:
	using StringView = std::experimental::string_view;

	struct PositionalFlagEntry
	{
		StringView desc;
	};

	struct OptionalFlagEntry
	{
		StringView desc;
		std::experimental::optional<StringView> defaultValue;
	};

	using OptionalFlagMap = VectorMap<StringView, OptionalFlagEntry>;
	OptionalFlagMap optFlagMap;
	using PositionalFlagMap = std::vector<std::pair<StringView, PositionalFlagEntry>>;
	PositionalFlagMap posFlagMap;

	StringView helpMsg;

	void addOptionalFlagEntry(const StringView& name, OptionalFlagEntry&& entry);
	void addPositionalFlagEntry(const StringView& name, PositionalFlagEntry&& entry);
	void parseArgs(const std::vector<StringView>& args, CommandLineFlags& result) const;
public:
	CommandLineParser(const StringView& h = ""): helpMsg(h) {}

	void addPositionalFlag(const StringView& name, const StringView& desc);
	void addOptionalFlag(const StringView& name, const StringView& desc);
	void addOptionalFlag(const StringView& name, const StringView& desc, const StringView& init);

	void printUsage(const StringView& progName) const;

	CommandLineFlags parseCommandLineOptions(int, char**) const;
};

}
