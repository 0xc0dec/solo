#ifndef __SOLO_LOG_H__
#define __SOLO_LOG_H__

#include <sstream>
#include <string>
#include "SoloCommonsInternal.h"

#define INFO(msg) solo::Log() << msg;
#define CRITICAL(msg) solo::Log() << msg;

namespace solo
{
	class Log
	{
	public:
		enum class LogLevel
		{
			INFO = 0,
			WARNING,
			ERROR,
			CRITICAL
		};
		
	public:
		Log(LogLevel level = LogLevel::INFO);
		~Log();

		Log &operator <<(const c8 *msg);
		Log &operator <<(s32 i);
		Log &operator <<(const str &s);

	private:
		LogLevel _level;
		std::ostringstream _buffer;
		
		str _getLevelString();
	};
}

#endif