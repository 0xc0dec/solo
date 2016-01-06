#pragma once

#include "SoloBase.h"

#define SL_LOG_DEBUG(...) solo::Log(SL_FMT(__VA_ARGS__), solo::LogLevel::Debug);
#define SL_LOG_INFO(...) solo::Log(SL_FMT(__VA_ARGS__), solo::LogLevel::Info);
#define SL_LOG_WARN(...) solo::Log(SL_FMT(__VA_ARGS__), solo::LogLevel::Warning);
#define SL_LOG_ERR(...) solo::Log(SL_FMT(__VA_ARGS__), solo::LogLevel::Error);
#define SL_LOG_CRITICAL(...) solo::Log(SL_FMT(__VA_ARGS__), solo::LogLevel::Critical);

namespace solo
{
    enum class LogLevel
    {
        Debug = 0,
        Info,
        Warning,
        Error,
        Critical
    };

    class Log
    {
    public:
        explicit Log(const std::string &msg, LogLevel level = LogLevel::Info);
        ~Log();

    private:
        LogLevel level;
        std::string message;

        std::string getLevelString();
    };
}
