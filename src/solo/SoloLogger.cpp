/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloLogger.h"
#include "SoloSpinLock.h"
#include <iostream>


namespace solo
{
    struct LoggerImpl
    {
        void log(const std::string &msg, const std::string &level)
        {
            volatile auto lt = lock.acquire();
            auto fullMsg = SL_FMT("[", level, "]	", msg);
            std::cout << fullMsg << std::endl;
            if (file.is_open())
                file << fullMsg << std::endl;
        }

        std::ofstream file;
        SpinLock lock;
    };
}


using namespace solo;


Logger::Logger(const FriendToken<Device>&)
{
    impl = std::make_shared<LoggerImpl>();
}


Logger::~Logger()
{
    if (impl->file.is_open())
        impl->file.close();
}


void Logger::setTargetFile(const std::string &path)
{
    if (impl->file.is_open())
        impl->file.close();
    if (!path.empty())
    {
        impl->file.open(path, std::ios_base::trunc);
        SL_PANIC_IF(!impl->file.is_open(), SL_FMT("Unable to open target log file ", path));
    }
}


void Logger::logDebug(const std::string &msg)
{
    impl->log(msg, "debug");
}


void Logger::logInfo(const std::string &msg)
{
    impl->log(msg, "info");
}


void Logger::logWarning(const std::string &msg)
{
    impl->log(msg, "warn");
}


void Logger::logError(const std::string &msg)
{
    impl->log(msg, "error");
}


void Logger::logCritical(const std::string &msg)
{
    impl->log(msg, "crit");
}
