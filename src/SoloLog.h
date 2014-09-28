#ifndef __SOLO_LOG_H__
#define __SOLO_LOG_H__

#include <sstream>
#include "SoloCommonsInternal.h"

#define INFO(msg) solo::Log() << msg;
#define CRITICAL(msg) solo::Log() << msg;

namespace solo
{
	class Log
	{
	public:
		~Log();

		Log &operator <<(const c8 *msg);
		Log &operator <<(s32 i);
		Log &operator <<(const str &s);

	private:
		std::ostringstream _buffer;
	};
}

#endif