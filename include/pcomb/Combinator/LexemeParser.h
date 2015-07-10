#ifndef PCOMB_LEXEME_PARSER_H
#define PCOMB_LEXEME_PARSER_H

#include "pcomb/Combinator/ParserAdapter.h"
#include "pcomb/Combinator/TokenParser.h"
#include "pcomb/Parser/PredicateCharParser.h"

namespace pcomb
{

// LexemeParser takes a parser p and returns a parser that parse an entire file using p. It tries to remove all preceding whitespaces first before sending the input to p, and after p succeeds it tries to remove trailing whitespaces and match an EOF.
template <typename ParserA>
auto lexeme(ParserA&& p, char finalChar, std::experimental::string_view whitespaces = " \t\n\v\f\r")
{
	return rule(
		seq(
			token(p, whitespaces),
			token(ch(finalChar), whitespaces)
		),
		[] (auto&& pair)
		{
			using ParserType = std::remove_reference_t<ParserA>;
			return std::forward<typename ParserType::OutputType>(std::get<0>(pair));
		}
	);
}

template <typename ParserA>
auto file(ParserA&& p)
{
	return lexeme(p, static_cast<char>(std::char_traits<char>::eof()));
}

template <typename ParserA>
auto line(ParserA&& p)
{
	return lexeme(p, '\n', " \t\r\v\f");
}

template <typename ParserA>
auto bigstr(ParserA&& p)
{
	return lexeme(p, '\0');
}

}

#endif
