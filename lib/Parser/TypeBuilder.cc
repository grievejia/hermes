#include "Parser/ErrorReport.h"
#include "Parser/TypeBuilder.h"
#include "pcomb/pcomb.h"

#include <unordered_set>

using namespace pcomb;

namespace hermes
{

static void validateSubtypeEntry(const TypeDescriptor& desc)
{
	std::unordered_set<std::experimental::string_view> names;
	for (auto const& entry: desc)
	{
		if (names.insert(entry.getName()).second)
		{
			reportError("Parsing error: Duplicate subtype ");
			reportError(entry.getName());
			reportError(" in type ");
			reportError(desc.getName());
			reportErrorAndTerminate("\n");
		}
	}
}

static void validateTypeEntry(const TypeDescriptorList& list)
{
	std::unordered_set<std::experimental::string_view> names;
	for (auto const& entry: list)
	{
		if (names.insert(entry.getName()).second)
		{
			reportError("Parsing error: Duplicate type ");
			reportError(entry.getName());
			reportErrorAndTerminate("\n");
		}
		validateSubtypeEntry(entry);
	}
}

TypeDescriptorList TypeBuilder::buildType(const StringView& input)
{
	auto id = regex("[a-zA-Z][\\w]+");
	auto fieldId = regex("[a-z][\\w]+");
	auto typeId = regex("[A-Z][\\w]+");

	auto fieldEntry = rule(
		seq(
			fieldId,
			token(ch(':')),
			token(id),
			token(ch(';'))
		),
		[] (auto&& tuple)
		{
			return std::make_pair(std::get<0>(tuple), std::get<2>(tuple));
		}
	);

	auto subtypeEntry = rule(
		seq(
			typeId,
			token(ch('{')),
			many(token(fieldEntry), 0),
			token(ch('}')),
			token(ch(';'))
		),
		[] (auto&& tuple)
		{
			auto stDescriptor = SubtypeDescriptor(std::get<0>(tuple).to_string());

			auto&& list = std::get<2>(tuple);
			for (auto const& entry: list)
			{
				auto succ = stDescriptor.addField(entry.first.to_string(), entry.second.to_string());
				if (!succ)
				{
					reportError("Parsing error: Duplicate field entry\n");
					reportError("In subtype ");
					reportError(std::get<0>(tuple).to_string());
					reportError(", field ");
					reportErrorAndTerminate(entry.first.to_string());reportError("Parsing error: Duplicate field entry\n");
					reportError("In subtype ");
					reportError(std::get<0>(tuple).to_string());
					reportError(", field ");
					reportError(entry.first.to_string());
					reportErrorAndTerminate("\n");
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
			many(token(subtypeEntry), 0),
			token(ch(';'))
		),
		[] (auto&& tuple)
		{
			auto typeDescriptor = TypeDescriptor(std::get<1>(tuple).to_string());

			for (auto&& entry: std::get<3>(tuple))
				typeDescriptor.addSubtype(std::move(entry));

			return typeDescriptor;
		}
	);

	auto typeEntries = file(many(typeEntry, 0));

	auto parseResult = typeEntries.parse(input);
	if (parseResult.hasError())
	{
		auto remainingStream = parseResult.getInputStream();
		reportError("Parsing failed at line ");
		reportError(std::to_string(remainingStream.getLineNumber()));
		reportError(", column ");
		reportError(std::to_string(remainingStream.getColumnNumber()));
		reportErrorAndTerminate("\n");
	}

	validateTypeEntry(parseResult.getOutput());
	return std::move(parseResult).getOutput();
}

}