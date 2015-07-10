#pragma once

#include "Util/CommandLine/CommandLineParser.h"

namespace util
{

class TypedCommandLineParser
{
private:
	using StringView = std::experimental::string_view;

	enum class OptionType
	{
		String,
		Bool,
		UInt,
	};

	struct OptionEntry
	{
		OptionType type;
		void* data;
	};

	VectorMap<StringView, OptionEntry> optionMap;

	CommandLineParser parser;

	void updateOptionMap(const StringView&, OptionType, void*);
public:
	TypedCommandLineParser(const StringView& h = ""): parser(h) {}

	void addBooleanOptionalFlag(const StringView&, const StringView&, bool&);
	void addStringOptionalFlag(const StringView&, const StringView&, StringView&);
	void addUIntOptionalFlag(const StringView&, const StringView&, unsigned&);
	void addStringPositionalFlag(const StringView&, const StringView&, StringView&);
	void parseCommandLineOptions(int argc, char** argv);
};

}
