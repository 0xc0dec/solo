#ifndef __SOLO_LOG_H__
#define __SOLO_LOG_H__

#include <sstream>
#include "SoloCommonInternal.h"

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

		Log &operator <<(const c8 *msg);
		Log &operator <<(s32 i);
		Log &operator <<(size_t s);
		Log &operator <<(const str &s);

	private:
		LogLevel _level;
		std::ostringstream _buffer;
		
		str _getLevelString();
	};
}

#endif