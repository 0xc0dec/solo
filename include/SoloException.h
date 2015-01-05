#pragma once

#include "SoloBase.h"

#define THROW(TExc, ...) throw TExc(FORMAT(__VA_ARGS__))

namespace solo
{
	class EngineException
	{
	public:
		std::string message;

		EngineException() {}
		EngineException(const std::string &msg) : message(msg) {}
	};
}
