#ifndef PCOMB_PARSER_H
#define PCOMB_PARSER_H

#include "pcomb/InputStream/InputStream.h"
#include "pcomb/Parser/ParseResult.h"

#include <type_traits>

namespace pcomb
{

template <typename O>
class Parser
{
public:
	using OutputType = O;
	using ResultType = ParseResult<OutputType>;

	virtual ~Parser() {}

	virtual ResultType parse(const InputStream& input) const = 0;
};

}

#endif
