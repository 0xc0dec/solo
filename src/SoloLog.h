#ifndef __SOLO_LOG_H__
#define __SOLO_LOG_H__

#include "SoloBaseInternal.h"

#define DEBUG(msg) solo::Log(solo::Log::LogLevel::Debug) << msg;
#define INFO(msg) solo::Log(solo::Log::LogLevel::Info) << msg;
#define WARN(msg) solo::Log(solo::Log::LogLevel::Warning) << msg;
#define ERROR(msg) solo::Log(solo::Log::LogLevel::Error) << msg;
#define CRITICAL(msg) solo::Log(solo::Log::LogLevel::Critical) << msg;

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

		explicit Log(LogLevel level = LogLevel::Info);
		~Log();

		Log &operator <<(const char *msg);
		Log &operator <<(int i);
		Log &operator <<(size_t s);
		Log &operator <<(const std::string &s);

	private:
		LogLevel _level;
		std::ostringstream _buffer;
		
		std::string _getLevelString();
	};
}

#endif