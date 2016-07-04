#pragma once

#include "SoloFormatter.h"
#include "SoloConfig.h"
#include <string>
#include <exception>
#include <cassert>


#define SL_ASSERT(condition) assert(condition)
#define SL_THROW(TExc, ...) throw TExc(__VA_ARGS__)
#define SL_FMT_THROW(TExc, ...) throw TExc(SL_FMT(__VA_ARGS__))

#define SL_THROW_IF(condition, exceptionType, ...) \
    do { \
        if (condition) \
            SL_THROW(exceptionType, __VA_ARGS__); \
    } while (0)

#define SL_SIMPLE_EXCEPTION(type) \
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

    SL_SIMPLE_EXCEPTION(InternalException)
    SL_SIMPLE_EXCEPTION(InvalidInputException)
    SL_SIMPLE_EXCEPTION(IOException)
    SL_SIMPLE_EXCEPTION(AssetException)

    class EffectCompilationException: public AssetException
    {
    public:
        std::string log;

        EffectCompilationException(const std::string& message, const std::string& log):
            AssetException(message), log(log)
        {
        }
    };
}
