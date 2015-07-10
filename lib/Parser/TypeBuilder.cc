#include "Parser/TypeBuilder.h"
#include "pcomb/pcomb.h"

#include <fstream>
#include <iostream>
#include <unordered_set>

using namespace pcomb;

namespace hermes
{

static void validateSubtypeEntry(const TypeDescriptor& desc)
{
	std::unordered_set<std::experimental::string_view> names;
	for (auto const& entry: desc)
	{
		if (!names.insert(entry.getName()).second)
		{
			std::cerr << "Specification error: Duplicate subtype \'" << entry.getName() << "\' in type \'" << desc.getName() << "\'\n";
			std::exit(-1);
		}
	}
}

static void validateTypeEntry(const TypeDescriptorList& list)
{
	if (list.empty())
	{
		std::cerr << "Specification error: type list is empty!\n";
		std::exit(-1);
	}

	std::unordered_set<std::experimental::string_view> names;
	for (auto const& entry: list)
	{
		if (!names.insert(entry.getName()).second)
		{
			std::cerr << "Specification error: Duplicate type \'" << entry.getName() << "\'\n";
			std::exit(-1);
		}
		validateSubtypeEntry(entry);
	}
}

TypeDescriptorList TypeBuilder::buildType(const StringView& input)
{
	auto id = regex("[a-zA-Z][\\w]+");
	auto fieldId = regex("[a-z][\\w]+");
	auto typeId = regex("[A-Z][\\w]+");
	auto fieldTypeId = regex("[a-zA-Z][:<>\\w]+[\\*&]?");

	auto fieldEntry = rule(
		seq(
			token(fieldId),
			token(ch(':')),
			token(fieldTypeId)
		),
		[] (auto&& tuple)
		{
			return std::make_pair(std::get<0>(tuple), std::get<2>(tuple));
		}
	);

	auto subtypeEntry = rule(
		seq(
			token(typeId),
			token(ch('{')),
			many(token(fieldEntry), 0),
			token(ch('}'))
		),
		[] (auto&& tuple)
		{
			auto stDescriptor = SubtypeDescriptor(std::get<0>(tuple));

			auto&& list = std::get<2>(tuple);
			for (auto const& entry: list)
			{
				auto succ = stDescriptor.addField(entry.first, entry.second);
				if (!succ)
				{
					std::cerr << "Specification error: Duplicate field entry\n  In subtype \'" << std::get<0>(tuple) << "\', field \'" << entry.first << "\'\n";
				}
			}

			return stDescriptor;
		}
	);

	auto typeEntry = rule(
		seq(
			token(str("type")),
			token(typeId),
			token(ch('=')),
			token(ch('{')),
			many(token(subtypeEntry), 0),
			token(ch('}')),
			token(ch(';'))
		),
		[] (auto&& tuple)
		{
			auto typeDescriptor = TypeDescriptor(std::get<1>(tuple));

			for (auto&& entry: std::get<4>(tuple))
				typeDescriptor.addSubtype(std::move(entry));

			return typeDescriptor;
		}
	);

	auto typeEntries = bigstr(many(typeEntry, 0));

	auto parseResult = typeEntries.parse(input);
	if (parseResult.hasError())
	{
		auto remainingStream = parseResult.getInputStream();
		std::cerr << "Parsing failed at line " << remainingStream.getLineNumber() << ", column " << remainingStream.getColumnNumber() << "\n";
		std::exit(-1);
	}

	validateTypeEntry(parseResult.getOutput());
	return std::move(parseResult).getOutput();
}

TypeDescriptorList TypeBuilder::loadTypeFromFile(const char* fileName)
{
	std::ifstream ifs(fileName, std::ios::in | std::ios::binary | std::ios::ate);

	auto fileSize = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	fileSize += std::streampos(1);
	std::vector<char> bytes(fileSize);
	ifs.read(&bytes[0], fileSize);

	return buildType(std::experimental::string_view(&bytes[0], fileSize));
}

}