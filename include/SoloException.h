#pragma once

#include "SoloBase.h"

#define THROW_FMT(TExc, ...) throw TExc(FORMAT(__VA_ARGS__))
#define THROW(TExc, ...) throw TExc(__VA_ARGS__)

namespace solo
{
	class EngineException
	{
	public:
		std::string message;

		EngineException() {}
		EngineException(const std::string &msg) : message(msg) {}
	};

	class EffectCompilationException: public EngineException
	{
	public:
		std::string log;

		EffectCompilationException() {}
		EffectCompilationException(const std::string& message, const std::string& log) : EngineException(message), log(log) {}
	};
}
