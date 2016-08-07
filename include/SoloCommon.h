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


#define SL_MACRO_BLOCK(code) do { code; } while (false);
#define SL_EMPTY_MACRO_BLOCK() do {} while (false);

/*
    Note
    Currently all error checks have the same priority. For instance, the same macro is used for places where only assert() is necessary.
    All checks are disabled in release build. Later they should probably get their respective "severity" level,
    making it possible to disable checks granularly.
*/

#ifdef SL_DEBUG
#   define SL_DBG_BLOCK(code) SL_MACRO_BLOCK(code)
#   define SL_ERR(...) SL_MACRO_BLOCK(throw EngineException(__VA_ARGS__))
#   define SL_ERR_IF(condition, ...) SL_MACRO_BLOCK(if (condition) throw EngineException(__VA_ARGS__))
#else
#   define SL_DBG_BLOCK(code) SL_EMPTY_MACRO_BLOCK()
#   define SL_ERR(...) SL_EMPTY_MACRO_BLOCK()
#   define SL_ERR_IF(condition, ...) SL_EMPTY_MACRO_BLOCK()
#endif


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
        std::string info = "";

        EngineException(): std::runtime_error("")
        {
        }

        explicit EngineException(const std::string& msg): std::runtime_error(msg)
        {
        }

        EngineException(const std::string& msg, const std::string& info):
            std::runtime_error(msg),
            info(info)
        {
        }
    };
}
