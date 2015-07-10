#pragma once

#include "Util/DataStructure/VectorMap.h"

#include <experimental/string_view>

namespace util
{

class CommandLineFlags
{
private:
	using MapType = VectorMap<std::experimental::string_view, std::experimental::string_view>;

	MapType flagMap;

	// Return true if insertion took place, false if override took place
	bool addFlag(const std::experimental::string_view& key, const std::experimental::string_view& value)
	{
		return flagMap.insert_or_assign(key, value).second;
	}
public:
	using const_iterator = MapType::const_iterator;

	CommandLineFlags() = default;

	const std::experimental::string_view* lookup(const std::experimental::string_view& key) const
	{
		auto itr = flagMap.find(key);
		if (itr == flagMap.end())
			return nullptr;
		else
			return &itr->second;
	}

	bool count(const std::experimental::string_view& key) const
	{
		return lookup(key) != nullptr;
	}

	const_iterator begin() const { return flagMap.begin(); }
	const_iterator end() const { return flagMap.end(); }

	friend class CommandLineParser;
};

}