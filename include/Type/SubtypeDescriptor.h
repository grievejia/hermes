#pragma once

#include "Util/DataStructure/VectorMap.h"

#include <experimental/string_view>

namespace hermes
{

class SubtypeDescriptor
{
private:
	using TypeName = std::string;
	TypeName name;

	using FieldName = std::string;
	using FieldType = std::string;
	using FieldMap = util::VectorMap<FieldName, FieldType>;
	FieldMap fieldMap;
public:
	using const_iterator = FieldMap::const_iterator;

	SubtypeDescriptor(const std::experimental::string_view& n): name(n.to_string()) {}

	const TypeName& getName() const { return name; }

	// Return false if the map already contains name
	bool addField(const std::experimental::string_view& name, const std::experimental::string_view& type)
	{
		auto result = fieldMap.try_emplace(name.to_string(), type.to_string());
		return result.second;
	}

	bool hasField(const FieldName& name) const
	{
		return fieldMap.count(name);
	}

	// Return NULL if field not exist
	const FieldType* getField(const FieldName& name) const
	{
		auto itr = fieldMap.find(name);
		if (itr != fieldMap.end())
			return &itr->second;
		else
			return nullptr;
	}

	const_iterator begin() const { return fieldMap.begin(); }
	const_iterator end() const { return fieldMap.end(); }
	size_t size() const { return fieldMap.size(); }
};

}