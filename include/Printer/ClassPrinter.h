#pragma once

#include "Printer/Printer.h"

#include <iosfwd>

namespace hermes
{

class ClassPrinter: public Printer<ClassPrinter>
{
private:
	void printTag(const TypeDescriptor&);
	void printBaseClass(const TypeDescriptor&);

	void printConstructor(const SubtypeDescriptor&, const std::string&);
	void printGetters(const SubtypeDescriptor&);
	void printPublicMethods(const SubtypeDescriptor&, const std::string&);
	void printPrivateFields(const SubtypeDescriptor&);
	void printSubClass(const SubtypeDescriptor&, const std::string&);
public:
	ClassPrinter(std::ostream& o): Printer(o) {}

	void print(const TypeDescriptor&);
};

}
