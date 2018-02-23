/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloFormatter.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <list>
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

#ifdef SL_DEBUG
#   define SL_DEBUG_BLOCK(code) SL_MACRO_BLOCK(code)
#   define SL_DEBUG_LOG(...) SL_MACRO_BLOCK(Logger::global().logDebug(SL_FMT(__VA_ARGS__, " (", __FILE__, ", line ", __LINE__, ")")))
#   define SL_DEBUG_PANIC(condition, ...) \
        SL_MACRO_BLOCK( \
            if (condition) \
            { \
                SL_DEBUG_LOG(__VA_ARGS__); \
                exit(1); \
            } \
        )
#else
#   define SL_DEBUG_BLOCK(code) SL_EMPTY_MACRO_BLOCK()
#   define SL_DEBUG_LOG(...) SL_EMPTY_MACRO_BLOCK()
#   define SL_DEBUG_PANIC(condition, ...) SL_EMPTY_MACRO_BLOCK()
#endif

namespace solo
{
    template <typename T> using sptr = std::shared_ptr<T>;
    template <typename T> using uptr = std::unique_ptr<T>;

    using str = std::string;

    template <typename T> using vec = std::vector<T>;
    template <typename T, size_t Size> using arr = std::array<T, Size>;
    template <typename T> using list = std::list<T>;
    template <class K, class V> using umap = std::unordered_map<K, V>;
    template <class K> using uset = std::unordered_set<K>;

    using s8 = char;
    using u8 = uint8_t;
    using u16 = uint16_t;
    using s32 = int32_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    class NoCopyAndMove
    {
    public:
        NoCopyAndMove() = default;
        virtual ~NoCopyAndMove() = default;

        NoCopyAndMove(const NoCopyAndMove &other) = delete;
        NoCopyAndMove(NoCopyAndMove &&other) = delete;
        auto operator=(const NoCopyAndMove &other) -> NoCopyAndMove& = delete;
        auto operator=(NoCopyAndMove &&other) -> NoCopyAndMove& = delete;
    };

    template <class T>
    class FriendToken
    {
        friend T;
        FriendToken() = default;
    };

    class Device;

    class Logger: public NoCopyAndMove
    {
    public:
        static auto global() -> Logger&;

        static auto create(const FriendToken<Device> &) -> sptr<Logger>;
        
        virtual ~Logger() = default;

        virtual void setOutputFile(const str &path) = 0;

        virtual void logDebug(const str &msg) = 0;
        virtual void logInfo(const str &msg) = 0;
        virtual void logWarning(const str &msg) = 0;
        virtual void logError(const str &msg) = 0;
        virtual void logCritical(const str &msg) = 0;

    protected:
        Logger() = default;
    };
}
