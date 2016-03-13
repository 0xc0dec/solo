#pragma once

#include "SoloFormatter.h"
#include "SoloConfig.h"
#include <string>
#include <exception>


#define SL_THROW(TExc, ...) throw TExc(__VA_ARGS__)
#define SL_FMT_THROW(TExc, ...) throw TExc(SL_FMT(__VA_ARGS__))

#ifdef SL_DEBUG
#   define SL_DEBUG_FMT_THROW(TExc, ...) SL_THROW_FMT(TExc, __VA_ARGS__)
#   define SL_DEBUG_FMT_THROW_IF(condition, exceptionType, ...) \
        do { \
            if (condition) \
                SL_FMT_THROW(exceptionType, __VA_ARGS__); \
        } while (0)

#   define SL_DEBUG_THROW(TExc, ...) SL_THROW(TExc, __VA_ARGS__)
#   define SL_DEBUG_THROW_IF(condition, exceptionType, ...) \
        do { \
            if (condition) \
                SL_THROW(exceptionType, __VA_ARGS__); \
        } while (0)
#else
#   define SL_DEBUG_FMT_THROW(TExc, ...)
#   define SL_DEBUG_THROW(TExc, ...)
#   define SL_DEBUG_FMT_THROW_IF(condition, exceptionType, ...)
#   define SL_DEBUG_THROW_IF(condition, exceptionType, ...)
#endif

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
    SL_SIMPLE_EXCEPTION(InvalidOperationException)
    SL_SIMPLE_EXCEPTION(InvalidInputException)
    SL_SIMPLE_EXCEPTION(IOException)
    SL_SIMPLE_EXCEPTION(ResourceException)

    class EffectCompilationException: public ResourceException
    {
    public:
        std::string log;

        EffectCompilationException(const std::string& message, const std::string& log) : ResourceException(message), log(log)
        {
        }
    };
}
