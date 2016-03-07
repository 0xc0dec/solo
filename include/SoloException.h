#pragma once

#include "SoloFormatter.h"
#include <string>
#include <exception>


#define SL_THROW(TExc, ...) throw TExc(__VA_ARGS__)
#define SL_THROW_FMT(TExc, ...) throw TExc(SL_FMT(__VA_ARGS__))

#define SL_DEFINE_SIMPLE_EXCEPTION(type) \
    class type: public EngineException \
    { \
    public: \
        explicit type(const std::string& msg) : EngineException(msg) \
        { \
        } \
    };

namespace solo
{
    class EngineException: public std::runtime_error
    {
    public:
        explicit EngineException(const std::string& msg) : std::runtime_error(msg)
        {
        }
    };

    SL_DEFINE_SIMPLE_EXCEPTION(InternalException)
    SL_DEFINE_SIMPLE_EXCEPTION(InvalidOperationException)
    SL_DEFINE_SIMPLE_EXCEPTION(InvalidInputException)
    SL_DEFINE_SIMPLE_EXCEPTION(IOException)
    SL_DEFINE_SIMPLE_EXCEPTION(ResourceException)

    class EffectCompilationException: public ResourceException
    {
    public:
        std::string log;

        EffectCompilationException(const std::string& message, const std::string& log) : ResourceException(message), log(log)
        {
        }
    };
}
