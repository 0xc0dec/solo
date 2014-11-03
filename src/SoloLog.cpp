#include <iostream>
#include "SoloLog.h"

using namespace solo;

Log::Log(LogLevel level)
	: _level(level)
{
}


Log::~Log()
{
	std::cout <<
		"[" << _getLevelString() << "]	" <<
		_buffer.str() <<
		std::endl;
}


str Log::_getLevelString()
{
	switch (_level)
	{
		case LogLevel::Debug:
			return "debug";
		case LogLevel::Info:
			return "info";
		case LogLevel::Warning:
			return "warn";
		case LogLevel::Error:
			return "error";
		case LogLevel::Critical:
			return "crit";
		default:
			return "";
	}
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


Log& Log::operator<<(size_t s)
{
	_buffer << s;
	return *this;
}


Log& Log::operator<<(const str &s)
{
	_buffer << s;
	return *this;
}