#include <iostream>

void reportError(const std::string& msg)
{
	std::cerr << msg;
}

void reportErrorAndTerminate(const std::string& msg)
{
	reportError(msg);
	std::exit(-1);
}
