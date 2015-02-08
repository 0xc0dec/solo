#pragma once

#include "SoloBase.h"

#define DEBUG(...) solo::Log(FORMAT(__VA_ARGS__), solo::Log::LogLevel::Debug);
#define INFO(...) solo::Log(FORMAT(__VA_ARGS__), solo::Log::LogLevel::Info);
#define WARN(...) solo::Log(FORMAT(__VA_ARGS__), solo::Log::LogLevel::Warning);
#define ERROR(...) solo::Log(FORMAT(__VA_ARGS__), solo::Log::LogLevel::Error);
#define CRITICAL(...) solo::Log(FORMAT(__VA_ARGS__), solo::Log::LogLevel::Critical);

namespace solo
{
	class Log
	{
	public:
		enum class LogLevel
		{
			Debug = 0,
			Info,
			Warning,
			Error,
			Critical
		};

		explicit Log(const std::string &msg, LogLevel level = LogLevel::Info);
		~Log();

	private:
		LogLevel level;
		std::string message;
		
		std::string getLevelString();
	};
}
