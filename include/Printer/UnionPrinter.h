#pragma once

#include "Printer/Printer.h"

namespace hermes
{

class UnionPrinter: public Printer<UnionPrinter>
{
private:
	void printBaseConstructor(const TypeDescriptor&);
	void printBaseDestructor(const TypeDescriptor&);
	void printBaseGetters(const TypeDescriptor&);
	void printBasePublicMethods(const TypeDescriptor&);
	void printBasePrivateFields(const TypeDescriptor&);
	void printTag(const TypeDescriptor&);
	void printBaseClass(const TypeDescriptor&);

	void printConstructor(const SubtypeDescriptor&);
	void printGetters(const SubtypeDescriptor&);
	void printPublicMethods(const SubtypeDescriptor&);
	void printPrivateFields(const SubtypeDescriptor&);
	void printSubClass(const SubtypeDescriptor&);
public:
	UnionPrinter(std::ostream& o): Printer(o) {}

	void print(const TypeDescriptor&);
};

}
