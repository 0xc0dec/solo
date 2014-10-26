#ifndef __SOLO_EXCEPTION_H__
#define __SOLO_EXCEPTION_H__

#include "SoloCommons.h"

namespace solo
{
	class EngineException
	{
	public:
		str message;

		EngineException()
		{
		}

		EngineException(const str &msg) : message(msg)
		{
		}
	};
}

#endif