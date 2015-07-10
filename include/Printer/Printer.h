#pragma once

#include "Type/TypeDescriptor.h"

#include <iosfwd>

namespace hermes
{

template <typename SubClass>
class Printer
{
protected:
	std::ostream& os;
public:
	Printer(std::ostream& o): os(o) {}

	void print(const TypeDescriptor& type)
	{
		static_cast<SubClass*>(this)->print(type);
	}

	void printList(const TypeDescriptorList& list)
	{
		for (auto const& elem: list)
		{
			this->print(elem);
			os << "\n";
		}
	}
};

}
