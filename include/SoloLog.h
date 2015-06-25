#pragma once

#include "SoloBase.h"

#define DEBUG(...) solo::Log(FORMAT(__VA_ARGS__), solo::LogLevel::Debug);
#define INFO(...) solo::Log(FORMAT(__VA_ARGS__), solo::LogLevel::Info);
#define WARN(...) solo::Log(FORMAT(__VA_ARGS__), solo::LogLevel::Warning);
#define ERROR(...) solo::Log(FORMAT(__VA_ARGS__), solo::LogLevel::Error);
#define CRITICAL(...) solo::Log(FORMAT(__VA_ARGS__), solo::LogLevel::Critical);

namespace solo
{
	enum class LogLevel
	{
		Debug = 0,
		Info,
		Warning,
		Error,
		Critical
	};

	class Log
	{
	public:
		explicit Log(const std::string &msg, LogLevel level = LogLevel::Info);
		~Log();

	private:
		LogLevel level;
		std::string message;
		
		std::string getLevelString();
	};
}
