/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloFormatter.h"
#include <string>
#include <memory>

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS)
#   define SL_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#   define SL_OSX
#elif defined(__linux__)
#   define SL_LINUX
#endif

#if defined(SL_WINDOWS) || defined(SL_OSX)
#   define SL_OPENGL_RENDERER
#endif

#ifdef SL_WINDOWS
#   define SL_VULKAN_RENDERER
#endif

#define SL_MACRO_BLOCK(code) do { code; } while (false);
#define SL_EMPTY_MACRO_BLOCK() do {} while (false);

/*
    TODO
    Currently all error checks have the same priority. For instance, the same macro is used for places where only assert() is necessary.
    All checks are disabled in release build. Later they should probably get their respective "severity" level,
    making it possible to disable checks granularly.
*/

#ifdef SL_DEBUG
#   define SL_ERR_CHECK
#endif

#ifdef SL_ERR_CHECK
#   define SL_PANIC_BLOCK(code) SL_MACRO_BLOCK(code)
#   define SL_PANIC(...) SL_MACRO_BLOCK(throw EngineException(__VA_ARGS__))
#   define SL_PANIC_IF(condition, ...) SL_MACRO_BLOCK(if (condition) throw EngineException(__VA_ARGS__))
#else
#   define SL_PANIC_BLOCK(code) SL_EMPTY_MACRO_BLOCK()
#   define SL_PANIC(...) SL_EMPTY_MACRO_BLOCK()
#   define SL_PANIC_IF(condition, ...) SL_EMPTY_MACRO_BLOCK()
#endif

#define SL_DISABLE_COPY_AND_MOVE(type) \
    type(const type &other) = delete; \
    type(type &&other) = delete; \
    type &operator=(const type &other) = delete; \
    type &operator=(type &&other) = delete;

namespace solo
{
    template <typename T> using sptr = std::shared_ptr<T>;
    template <typename T> using uptr = std::unique_ptr<T>;
    using str = std::string;

    class EngineException: public std::runtime_error
    {
    public:
        EngineException(): std::runtime_error("")
        {
        }

        explicit EngineException(const str &msg): std::runtime_error(msg)
        {
        }
    };

    template <class T>
    class FriendToken // TODO Remove - not needed since we moved everything to scripts
    {
        friend T;
        FriendToken() {}
    };
}
