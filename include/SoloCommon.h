#pragma once

#include "SoloConfig.h"
#include "SoloFormatter.h"
#include <memory>
#include <cassert>


#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS)
#   define SL_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#   define SL_MACOS
#elif defined(__linux__)
#   define SL_LINUX
#endif

#if defined(SL_WINDOWS) || defined(SL_MACOS)
#   define SL_OPENGL_RENDERER
#endif

#if defined(SL_WINDOWS)
#   define SL_VULKAN_RENDERER
#endif


#ifdef SL_DEBUG
#   define SL_IN_DEBUG(code) code
#else
#   define SL_IN_DEBUG(code)
#endif

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
        type() : EngineException("") \
        { \
        } \
        \
        explicit type(const std::string& msg) : EngineException(msg) \
        { \
        } \
    };


#define SL_NONCOPYABLE(type) \
    type(const type &other) = delete; \
    type(type &&other) = delete; \
    type &operator=(const type &other) = delete; \
    type &operator=(type &&other) = delete;


#define SL_FLUENT_DTO(dtoType, fields) \
    struct dtoType final \
    { \
        typedef dtoType DtoType; \
        fields \
    };

#define SL_FLUENT_DTO_FIELD(fieldType, fieldName, setterMethodName, defaultValue) \
    fieldType fieldName defaultValue; \
    auto setterMethodName(fieldType fieldName) -> DtoType& \
    { \
        this->fieldName = fieldName; \
        return *this; \
    }

namespace solo
{
    template <typename T> using sptr = std::shared_ptr<T>;
    template <typename T> using uptr = std::unique_ptr<T>;

    class EngineException: public std::runtime_error
    {
    public:
        EngineException(): std::runtime_error("")
        {
        }

        explicit EngineException(const std::string& msg): std::runtime_error(msg)
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
