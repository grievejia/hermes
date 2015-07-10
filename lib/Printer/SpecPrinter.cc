#include "Printer/SpecPrinter.h"

#include <ostream>

namespace hermes
{

void SpecPrinter::printSubtype(const SubtypeDescriptor& desc)
{
	os << "  " << desc.getName() << "\n  {\n";

	for (auto const& mapping: desc)
	{
		os << "    " << mapping.first << " : " << mapping.second << "\n";
	}

	os << "  }\n";
}

void SpecPrinter::print(const TypeDescriptor& desc)
{
	os << "type " << desc.getName() << " = \n{\n";

	for (auto const& elem: desc)
		printSubtype(elem);

	os << "}\n";
}

}