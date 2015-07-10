#include "Printer/ClassPrinter.h"
#include "Util/Helper.h"

#include <ostream>

namespace hermes
{

void ClassPrinter::printTag(const TypeDescriptor& type)
{
	os << "\tenum class Tag: " << getUnderlyingType(type.size()) << "\n\t{\n";

	for (auto const& subtype: type)
		os << "\t\t" << subtype.getName() << ",\n";

	os << "\t};\n";
}

void ClassPrinter::printBaseClass(const TypeDescriptor& type)
{
	os << "class " << type.getName() << "\n{\n";
	os << "public:\n";
	printTag(type);

	os << "private:\n";
	os << "\tTag tag;\n";

	os << "public:\n";
	os << "\t" << type.getName() << "(Tag t): tag(t) {}\n";
	os << "\tvirtual ~" << type.getName() << "() = 0;\n";
	os << "\tTag getTag() const { return tag; }\n";

	for (auto const& subtype: type)
		os << "\tbool is" << subtype.getName() << "() const { return tag == Tag::" << subtype.getName() << "; }\n";

	os << "};\n\n";
	os << "inline " << type.getName() << "::~" << type.getName() << "() = default;\n";
}

void ClassPrinter::printPrivateFields(const SubtypeDescriptor& desc)
{
	os << "private:\n";
	for (auto const& mapping: desc)
	{
		os << "\t" << mapping.second << " " << mapping.first << ";\n";
	}
}

void ClassPrinter::printConstructor(const SubtypeDescriptor& desc, const std::string& baseName)
{
	os << "\t" << desc.getName() << "(";

	unsigned id = 0;
	for (auto const& mapping: desc)
	{
		if (id > 0)
			os << ", ";
		os << mapping.second << " " << getTempVariableName(id);
		++id;
	}

	os << "):\n\t\t" << baseName << "(" << baseName << "::Tag::" << desc.getName() << "),\n\t\t";

	id = 0;
	for (auto const& mapping: desc)
	{
		if (id > 0)
			os << ",\n\t\t";
		os << mapping.first << "(" << getTempVariableName(id) << ")";
		++id;
	}

	os << "\n\t{}\n";
}

void ClassPrinter::printGetters(const SubtypeDescriptor& desc)
{
	for (auto const& mapping: desc)
	{
		os << "\t" << getConstRefType(mapping.second) << " get" << capitalize(mapping.first) << "() const { return " << mapping.first << "; }\n";
	}
}

void ClassPrinter::printPublicMethods(const SubtypeDescriptor& desc, const std::string& baseName)
{
	os << "public:\n";
	printConstructor(desc, baseName);
	printGetters(desc);
}

void ClassPrinter::printSubClass(const SubtypeDescriptor& desc, const std::string& baseName)
{
	os << "class " << desc.getName() << ": public " << baseName << "\n{\n";

	printPrivateFields(desc);
	printPublicMethods(desc, baseName);

	os << "};\n";
}

void ClassPrinter::print(const TypeDescriptor& desc)
{
	printBaseClass(desc);
	os << "\n";

	for (auto const& subclass: desc)
	{
		printSubClass(subclass, desc.getName());
		os << "\n";
	}
}

}