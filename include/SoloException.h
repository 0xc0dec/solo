#pragma once

#include "SoloFormatter.h"
#include "SoloConfig.h"
#include <string>
#include <exception>


#define SL_THROW(TExc, ...) throw TExc(__VA_ARGS__)
#define SL_THROW_FMT(TExc, ...) throw TExc(SL_FMT(__VA_ARGS__))

#ifdef SL_DEBUG
#   define SL_DEBUG_FMT_THROW_IF(condition, exceptionType, ...) \
        { \
            if (condition) \
                SL_THROW_FMT(exceptionType, __VA_ARGS__); \
        }

#   define SL_DEBUG_THROW_IF(condition, exceptionType, ...) \
        { \
            if (condition) \
                SL_THROW(exceptionType, __VA_ARGS__); \
        }
#else
#   define SL_DEBUG_FMT_THROW_IF(condition, exceptionType, exceptionMessage) {}
#endif

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
