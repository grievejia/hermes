#pragma once

#include "Type/TypeDescriptor.h"

#include <experimental/string_view>
#include <iosfwd>

namespace hermes
{

class TypeBuilder
{
private:
	using StringView = std::experimental::string_view;
public:
	static TypeDescriptorList buildType(const StringView&);
	static TypeDescriptorList loadTypeFromFile(std::ifstream&);
};

}