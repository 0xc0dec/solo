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
		case LogLevel::INFO:
			return "info";
		case LogLevel::WARNING:
			return "warn";
		case LogLevel::ERROR:
			return "error";
		case LogLevel::CRITICAL:
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


Log& Log::operator<<(const str &s)
{
	_buffer << s;
	return *this;
}