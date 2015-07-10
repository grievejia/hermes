#pragma once

#include <string>
#include <unordered_set>

namespace hermes
{

inline std::string getTempVariableName(unsigned id)
{
	return "p" + std::to_string(id);
}

inline std::string capitalize(const std::string& str)
{
	if (str.empty())
		return "";
	else
	{
		std::string ret;
		ret.reserve(str.size());
		ret.push_back(std::toupper(str[0]));
		ret.append(str.substr(1));
		return std::move(ret);
	}
}

inline std::string decapitalize(const std::string& str)
{
	if (str.empty())
		return "";
	else
	{
		std::string ret;
		ret.reserve(str.size());
		ret.push_back(std::tolower(str[0]));
		ret.append(str.substr(1));
		return std::move(ret);
	}
}

inline std::string getUnderlyingType(size_t size)
{
	if (size <= 2)
		return "bool";
	else if (size <= 256)
		return "std::uint8_t";
	else if (size <= 65536)
		return "std::uint16_t";
	else if (size <= 4294967296)
		return "std::uint32_t";
	else
		return "std::uint64_t";
}

inline bool isPrimitiveType(const std::string& typeName)
{
	static std::unordered_set<std::string> primSet = { "bool", "char", "short", "unisgned", "int", "long", "size_t", "ssize_t", "uint8_t", "int8_t", "uint16_t", "int16_t", "uint32_t", "int32_t", "uint64_t", "int64_t", "float", "double"};
	return primSet.count(typeName);
}

inline bool isPointerType(const std::string& typeName)
{
	return !typeName.empty() && typeName.back() == '*';
}

inline bool isRefType(const std::string& typeName)
{
	return !typeName.empty() && typeName.back() == '&';
}

inline std::string getConstRefType(const std::string& typeName)
{
	if (isPrimitiveType(typeName))
		return typeName;
	else if (isPointerType(typeName) || isRefType(typeName))
		return "const " + typeName;
	else
		return "const " + typeName + "&";
}

}