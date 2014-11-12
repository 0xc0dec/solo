#ifndef __SOLO_EXCEPTION_H__
#define __SOLO_EXCEPTION_H__

#include "SoloCommon.h"

namespace solo
{
	class EngineException
	{
	public:
		String message;

		EngineException()
		{
		}

		EngineException(const String &msg) : message(msg)
		{
		}
	};
}

#endif