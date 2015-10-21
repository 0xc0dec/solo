#include <iostream>
#include "SoloLog.h"

using namespace solo;


Log::Log(const std::string &msg, LogLevel level):
	level(level), message(msg)
{
}


Log::~Log()
{
	std::cout << SL_FMT("[", getLevelString(), "]	", message) << std::endl;
}


std::string Log::getLevelString()
{
	switch (level)
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
