#include <iostream>
#include "SoloLog.h"

using namespace solo;

Log::~Log()
{
	std::cout << _buffer.str() << std::endl;
}

Log &Log::operator<<(const c8 *msg)
{
	_buffer << msg;
	return *this;
}


Log& Log::operator<<(s32 i)
{
	_buffer << i;
	return *this;
}


Log& Log::operator<<(const str &s)
{
	_buffer << s;
	return *this;
}