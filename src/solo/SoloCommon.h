/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <list>
#include <memory>
#include <functional>
#include <sstream>

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

    class Device;

    class Logger
    {
    public:
        static auto global() -> Logger&;

        Logger(const Logger &other) = delete;
        Logger(Logger &&other) = delete;
        virtual ~Logger() = default;
    	
        auto operator=(const Logger &other) -> Logger& = delete;
        auto operator=(Logger &&other) -> Logger& = delete;

        virtual void setOutputFile(const str &path) = 0;

        virtual void logDebug(const str &msg) = 0;
        virtual void logInfo(const str &msg) = 0;
        virtual void logWarning(const str &msg) = 0;
        virtual void logError(const str &msg) = 0;
        virtual void logCritical(const str &msg) = 0;

    protected:
        Logger() = default;
    };

	template <class... TArgs>
	constexpr void asrt(bool condition, TArgs ... msgArgs)
	{
#ifdef SL_DEBUG
		if (!condition)
		{
			const auto msg = fmt(msgArgs...);
			Logger::global().logDebug(msg);
			exit(1);
		}
#endif
	}

	template <class T>
	constexpr auto fmt(std::ostringstream &out, T &&arg) -> str
	{
		out << arg << std::endl;
		return out.str();
	}

	template <class TFirst, class... TArgs>
	constexpr auto fmt(std::ostringstream &out, TFirst &&first, TArgs&& ...args) -> str
	{
		out << first;
		return fmt(out, std::forward<TArgs>(args)...);
	}

	template <class... TArgs>
	constexpr auto fmt(TArgs&& ...args) -> str
	{
		std::ostringstream out;
		return fmt(out, std::forward<TArgs>(args)...);
	}

	template <class... TArgs>
	constexpr void asrt(const std::function<bool()> &condition, TArgs&& ...msgArgs)
	{
#ifdef SL_DEBUG
		asrt(condition(), std::forward<TArgs>(msgArgs)...);
#endif
	}

	inline void asrt(const std::function<void()> &check)
	{
#ifdef SL_DEBUG
		check();
#endif
	}

	template <class... TArgs>
	constexpr void panic(TArgs&& ...args)
	{
		const auto msg = fmt(std::forward<TArgs>(args)...);
		Logger::global().logCritical(msg);
		exit(1);
	}

	template <class... TArgs>
	constexpr void panicIf(bool condition, TArgs&& ...args)
	{
		if (condition)
			panic(std::forward<TArgs>(args)...);
	}
}
