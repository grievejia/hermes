#pragma once

#include "Printer/Printer.h"

#include <iosfwd>

namespace hermes
{

class SpecPrinter: public Printer<SpecPrinter>
{
private:
	void printSubtype(const SubtypeDescriptor&);
public:
	SpecPrinter(std::ostream& o): Printer(o) {}

	void print(const TypeDescriptor&);
};

}
