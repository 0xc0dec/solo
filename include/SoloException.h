#ifndef __SOLO_EXCEPTION_H__
#define __SOLO_EXCEPTION_H__

#include <string>

namespace solo
{
	class EngineException
	{
	public:
		std::string message;

		EngineException()
		{
		}

		EngineException(const std::string msg) : message(msg)
		{
		}
	};
}

#endif