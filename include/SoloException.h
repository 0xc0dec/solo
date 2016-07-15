#pragma once

#include "SoloFormatter.h"
#include "SoloConfig.h"
#include <string>
#include <exception>
#include <cassert>


#define SL_ASSERT(condition) assert(condition)
#define SL_EXCEPTION(TExc, ...) throw TExc(__VA_ARGS__)
#define SL_FMT_EXCEPTION(TExc, ...) throw TExc(SL_FMT(__VA_ARGS__))

#define SL_EXCEPTION_IF(condition, exceptionType, ...) \
    do { \
        if (condition) \
            SL_EXCEPTION(exceptionType, __VA_ARGS__); \
    } while (0)

#define SL_SIMPLE_EXCEPTION(type, final_) \
    class type final_: public EngineException \
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

    SL_SIMPLE_EXCEPTION(InternalException, final)
    SL_SIMPLE_EXCEPTION(InvalidInputException, final)
    SL_SIMPLE_EXCEPTION(IOException, final)
    SL_SIMPLE_EXCEPTION(AssetException, )

    class EffectCompilationException final: public AssetException
    {
    public:
        std::string log;

        EffectCompilationException(const std::string& message, const std::string& log):
            AssetException(message), log(log)
        {
        }
    };
}
