/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloCommon.h"
#include "SoloSpinLock.h"
#include <iostream>
#include <fstream>

using namespace solo;

namespace solo
{
    class LoggerImpl : public Logger
    {
    public:
        ~LoggerImpl()
        {
            if (file.is_open())
                file.close();
        }

        void setOutputFile(const str &path) override final
        {
            if (file.is_open())
                file.close();
            if (!path.empty())
            {
                file.open(path, std::ios_base::trunc);
                panicIf(!file.is_open(), "Unable to open log file ", path);
            }
        }

        void logDebug(const str &msg) override final
        {
            log(msg, "debug");
        }

        void logInfo(const str &msg) override final
        {
            log(msg, "info");
        }

        void logWarning(const str &msg) override final
        {
            log(msg, "warn");
        }

        void logError(const str &msg) override final
        {
            log(msg, "error");
        }

        void logCritical(const str &msg) override final
        {
            log(msg, "crit");
        }

    private:
        std::ofstream file;
        SpinLock lock;

        void log(const str &msg, const str &level)
        {
            volatile auto lt = lock.acquire();
            const auto fullMsg = fmt("[", level, "] ", msg);
            std::cout << fullMsg << std::endl;
            if (file.is_open())
                file << fullMsg << std::endl;
        }
    };
}

auto Logger::global() -> Logger &
{
    static LoggerImpl instance;
    return instance;
}
