#pragma once

#include "Type/SubtypeDescriptor.h"

#include <vector>

namespace hermes
{

class TypeDescriptor
{
private:
	using TypeName = std::string;
	TypeName name;

	using SubtypeList = std::vector<SubtypeDescriptor>;
	SubtypeList subtypes;
public:
	using const_iterator = SubtypeList::const_iterator;

	TypeDescriptor(const std::experimental::string_view& n): name(n.to_string()) {}

	const TypeName& getName() const { return name; }

	void addSubtype(SubtypeDescriptor&& s)
	{
		subtypes.emplace_back(std::move(s));
	}

	const_iterator begin() const { return subtypes.begin(); }
	const_iterator end() const { return subtypes.end(); }
	size_t size() const { return subtypes.size(); }
};

using TypeDescriptorList = std::vector<TypeDescriptor>;

}