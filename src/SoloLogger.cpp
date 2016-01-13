#include "SoloLogger.h"
#include <iostream>

using namespace solo;


shared<Logger> Logger::create()
{
    return SL_NEW_SHARED(Logger);
}


Logger::~Logger()
{
    if (targetFileStream.is_open())
        targetFileStream.close();
}


void Logger::setTargetFile(const std::string& path)
{
    if (targetFileStream.is_open())
        targetFileStream.close();
    if (!path.empty())
    {
        targetFileStream.open(path, std::ios_base::trunc);
        if (!targetFileStream.is_open())
            logError(SL_FMT("Unable to open target log file ", path));
    }
}


void Logger::logDebug(const std::string& msg)
{
    log(msg, "debug");
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
    auto fullMsg = SL_FMT("[", level, "]	", msg);
    std::cout << fullMsg << std::endl;
    if (targetFileStream.is_open())
        targetFileStream << fullMsg << std::endl;
}
