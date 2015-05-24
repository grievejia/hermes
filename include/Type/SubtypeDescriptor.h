#pragma once

#include <string>
#include <unordered_map>

namespace hermes
{

class SubtypeDescriptor
{
private:
	using TypeName = std::string;
	TypeName name;

	using FieldName = std::string;
	using FieldType = std::string;
	using FieldMap = std::unordered_map<FieldName, FieldType>;
	FieldMap fieldMap;
public:
	using const_iterator = FieldMap::const_iterator;

	SubtypeDescriptor(const TypeName& n): name(n) {}
	SubtypeDescriptor(TypeName&& n): name(std::move(n)) {}

	const TypeName& getName() const { return name; }

	// Return false if the map already contains name
	bool addField(FieldName&& name, FieldType&& type)
	{
		auto itr = fieldMap.find(name);
		if (itr != fieldMap.end())
			return false;
		
		fieldMap.insert(itr, std::make_pair(std::move(name), std::move(type)));
		return true;
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
};

}