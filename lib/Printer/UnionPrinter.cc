#include "Printer/UnionPrinter.h"
#include "Util/Helper.h"

#include <ostream>

namespace hermes
{

void UnionPrinter::printPrivateFields(const SubtypeDescriptor& desc)
{
	os << "private:\n";
	for (auto const& mapping: desc)
	{
		os << "\t" << mapping.second << " " << mapping.first << ";\n";
	}
}

void UnionPrinter::printConstructor(const SubtypeDescriptor& desc)
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

	os << "):\n\t\t";

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

void UnionPrinter::printGetters(const SubtypeDescriptor& desc)
{
	for (auto const& mapping: desc)
	{
		os << "\t" << getConstRefType(mapping.second) << " get" << capitalize(mapping.first) << "() const { return " << mapping.first << "; }\n";
	}
}

void UnionPrinter::printPublicMethods(const SubtypeDescriptor& desc)
{
	os << "public:\n";
	printConstructor(desc);
	printGetters(desc);
}

void UnionPrinter::printSubClass(const SubtypeDescriptor& desc)
{
	os << "class " << desc.getName() << "\n{\n";

	printPrivateFields(desc);
	printPublicMethods(desc);

	os << "};\n";
}

void UnionPrinter::printBaseConstructor(const TypeDescriptor& type)
{
	for (auto const& subtype: type)
	{
		os << "\tstruct " << subtype.getName() << "Cons {};\n";
		os << "\t" << type.getName() << "(" << subtype.getName() << "Cons, ";

		unsigned id = 0;
		for (auto const& mapping: subtype)
		{
			if (id > 0)
				os << ", ";
			os << mapping.second << " " << getTempVariableName(id);
			++id;
		}

		os << "): tag(Tag::" << subtype.getName() << ")";
		os << "\n\t{\n";

		os << "\t\tnew (&" << decapitalize(subtype.getName()) << ") " << subtype.getName() << "(";
		for (id = 0; id < subtype.size(); ++id)
		{
			if (id > 0)
				os << ", ";
			os << getTempVariableName(id);
		}

		os << ");\n\t}\n\n";
	}
}

void UnionPrinter::printBaseGetters(const TypeDescriptor& type)
{
	for (auto const& subtype: type)
	{
		os << "\tbool is" << subtype.getName() << "() const { return tag == Tag::" << subtype.getName() << "; }\n";

		os << "\t" << getConstRefType(subtype.getName()) << " getAs" << subtype.getName() << "() const\n\t{\n";
		os << "\t\tassert(is" << subtype.getName() << "() && \"" << type.getName() << " must be a " << subtype.getName() << "!\");\n";
		os << "\t\treturn " << decapitalize(subtype.getName()) << ";\n";
		os << "\t}\n";

		os << "\tstatic " << type.getName() << " get" << subtype.getName() << "(";
		unsigned id = 0;
		for (auto const& mapping: subtype)
		{
			if (id > 0)
				os << ", ";
			os << mapping.second << " " << getTempVariableName(id);
			++id;
		}
		os << ")\n\t{\n";
		os << "\t\treturn " << type.getName() << "(" << subtype.getName() << "Cons()";
		for (id = 0; id < subtype.size(); ++id)
		{
			os << ", " << getTempVariableName(id);
		}
		os << ");\n";
		os << "\t}\n";

		os << "\n";
	}
}

void UnionPrinter::printBaseDestructor(const TypeDescriptor& type)
{
	os << "\t~" << type.getName() << "()\n\t{\n";

	os << "\t\tswitch (tag)\n\t\t{\n";
	for (auto const& subtype: type)
	{
		os << "\t\t\tcase Tag::" << subtype.getName() << ":\n";
		os << "\t\t\t\t" << decapitalize(subtype.getName()) << ".~" << subtype.getName() << "();\n";
		os << "\t\t\t\tbreak;\n";
	}
	os << "\t\t}\n";

	os << "\t}\n";
}

void UnionPrinter::printBasePublicMethods(const TypeDescriptor& type)
{
	os << "public:\n";
	printBaseGetters(type);
	printBaseDestructor(type);
}

void UnionPrinter::printBasePrivateFields(const TypeDescriptor& type)
{
	os << "private:\n";
	os << "\tTag tag;\n";
	os << "\tunion\n\t{\n";
	for (auto const& subtype: type)
		os << "\t\t" << subtype.getName() << " " << decapitalize(subtype.getName()) << ";\n";
	os << "\t};\n\n";

	// Create one type for each union field to dispatch the constructor
	printBaseConstructor(type);
}

void UnionPrinter::printTag(const TypeDescriptor& type)
{
	os << "public:\n\tenum class Tag: " << getUnderlyingType(type.size()) << "\n\t{\n";

	for (auto const& subtype: type)
		os << "\t\t" << subtype.getName() << ",\n";

	os << "\t};\n";
}

void UnionPrinter::printBaseClass(const TypeDescriptor& type)
{
	os << "class " << type.getName() << "\n{\n";
	printTag(type);

	printBasePrivateFields(type);
	printBasePublicMethods(type);
	os << "};\n";
}

void UnionPrinter::print(const TypeDescriptor& type)
{
	os << "#include <cassert>\n";
	os << "#include <new>\n";

	for (auto const& subtype: type)
	{
		printSubClass(subtype);
		os << "\n";
	}

	printBaseClass(type);
}

}
