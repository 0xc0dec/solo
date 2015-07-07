#pragma once

#include "SoloBase.h"

#define THROW_FMT(TExc, ...) throw TExc(FORMAT(__VA_ARGS__))
#define THROW(TExc, ...) throw TExc(__VA_ARGS__)

namespace solo
{
	class EngineException: public std::runtime_error
	{
	public:
		EngineException(const std::string &msg) : std::runtime_error(msg)
		{
		}
	};

	class EffectCompilationException: public EngineException
	{
	public:
		std::string log;

		EffectCompilationException(const std::string& message, const std::string& log) : EngineException(message), log(log) {}
	};
}
