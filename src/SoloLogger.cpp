#include "SoloLogger.h"
#include <iostream>

using namespace solo;


Logger::~Logger()
{
    if (file.is_open())
        file.close();
}


void Logger::setTargetFile(const std::string& path)
{
    if (file.is_open())
        file.close();
    if (!path.empty())
    {
        file.open(path, std::ios_base::trunc);
        SL_ERR_IF(!file.is_open(), SL_FMT("Unable to open target log file ", path));
    }
}


void Logger::logDebug(const std::string& msg)
{
#ifdef SL_DEBUG
    log(msg, "debug");
#endif
}


void Logger::logInfo(const std::string& msg)
{
    log(msg, "info");
}


void Logger::logWarning(const std::string& msg)
{
    log(msg, "warn");
}


void Logger::logError(const std::string& msg)
{
    log(msg, "error");
}


void Logger::logCritical(const std::string& msg)
{
    log(msg, "crit");
}


void Logger::log(const std::string& msg, const std::string& level)
{
    auto lt = lock.acquire();
    auto fullMsg = SL_FMT("[", level, "]	", msg);
    std::cout << fullMsg << std::endl;
    if (file.is_open())
        file << fullMsg << std::endl;
}
