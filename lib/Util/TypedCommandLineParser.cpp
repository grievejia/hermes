#include "Util/CommandLine/TypedCommandLineParser.h"

namespace util
{

void TypedCommandLineParser::updateOptionMap(const StringView& name, const OptionType type, void* data)
{
	auto inserted = optionMap.try_emplace(name, OptionEntry{type, data}).second;
	assert(inserted && "Duplicate entry in optionMap!");
}

void TypedCommandLineParser::addUIntOptionalFlag(const StringView& name, const StringView& desc, unsigned& flag)
{
	parser.addOptionalFlag(name, desc, "");
	updateOptionMap(name, OptionType::UInt, &flag);
}

void TypedCommandLineParser::addBooleanOptionalFlag(const StringView& name, const StringView& desc, bool& flag)
{
	parser.addOptionalFlag(name, desc);
	updateOptionMap(name, OptionType::Bool, &flag);
}

void TypedCommandLineParser::addStringOptionalFlag(const StringView& name, const StringView& desc, StringView& str)
{
	parser.addOptionalFlag(name, desc, "");
	updateOptionMap(name, OptionType::String, &str);
}

void TypedCommandLineParser::addStringPositionalFlag(const StringView& name, const StringView& desc, StringView& str)
{
	parser.addPositionalFlag(name, desc);
	updateOptionMap(name, OptionType::String, &str);
}

void TypedCommandLineParser::parseCommandLineOptions(int argc, char** argv)
{
	auto parseResult = parser.parseCommandLineOptions(argc, argv);

	for (auto& mapping: optionMap)
	{
		auto data = mapping.second.data;
		switch (mapping.second.type)
		{
			case OptionType::Bool:
			{
				*(reinterpret_cast<bool*>(data)) = parseResult.count(mapping.first);
				break;
			}
			case OptionType::UInt:
			{
				auto res = parseResult.lookup(mapping.first);
				assert(res != nullptr);
				if (!res->empty())
					*(reinterpret_cast<unsigned*>(data)) = std::stoul(res->to_string());
				break;
			}
			case OptionType::String:
			{
				auto res = parseResult.lookup(mapping.first);
				assert(res != nullptr);
				if (!res->empty())
					*(reinterpret_cast<StringView*>(data)) = *res;
				break;
			}
		}
	}
}

}
