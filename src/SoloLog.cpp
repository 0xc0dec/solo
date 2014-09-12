#include <iostream>
#include "SoloLog.h"

using namespace solo;

Log::~Log()
{
	std::cout << _buffer.str() << std::endl;
}

Log &Log::operator<<(char const* msg)
{
	_buffer << msg;
	return *this;
}


Log& Log::operator<<(int i)
{
	_buffer << i;
	return *this;
}


Log& Log::operator<<(std::string const& s)
{
	_buffer << s;
	return *this;
}