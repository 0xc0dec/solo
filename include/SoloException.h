#pragma once

#include "SoloFormatter.h"
#include <string>
#include <exception>

#define SL_THROW(TExc, ...) throw TExc(__VA_ARGS__)
#define SL_THROW_FMT(TExc, ...) throw TExc(SL_FMT(__VA_ARGS__))

namespace solo
{
    class EngineException: public std::runtime_error
    {
    public:
        explicit EngineException(const std::string &msg) : std::runtime_error(msg)
        {
        }
    };

    class EffectCompilationException: public EngineException
    {
    public:
        std::string log;

        EffectCompilationException(const std::string &message, const std::string &log) : EngineException(message), log(log) {}
    };
}
