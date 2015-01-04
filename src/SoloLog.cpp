#include <iostream>
#include "SoloLog.h"

using namespace solo;

Log::Log(const std::string &msg, LogLevel level)
	: _level(level),
	_message(msg)
{
}


Log::~Log()
{
	std::cout << FORMAT("[", _getLevelString(), "]	", _message) << std::endl;
}


std::string Log::_getLevelString()
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
