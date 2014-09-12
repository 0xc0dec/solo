#ifndef __SOLO_LOG_H__
#define __SOLO_LOG_H__

#include <sstream>
#include <string>

#define INFO(msg) solo::Log() << msg;
#define CRITICAL(msg) solo::Log() << msg;

namespace solo
{
	class Log
	{
	public:
		~Log();

		Log &operator <<(const char *msg);
		Log &operator <<(int i);
		Log &operator <<(const std::string &s);

	private:
		std::ostringstream _buffer;
	};
}

#endif