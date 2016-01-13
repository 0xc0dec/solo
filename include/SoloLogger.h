#pragma once

#include "SoloBase.h"
#include <fstream>

namespace solo
{
    class Logger
    {
    public:
        static shared<Logger> create();

        SL_NONCOPYABLE(Logger);
        ~Logger();

        void setTargetFile(const std::string& path);

        void logDebug(const std::string& msg);
        void logInfo(const std::string& msg);
        void logWarning(const std::string& msg);
        void logError(const std::string& msg);
        void logCritical(const std::string& msg);

    private:
        Logger() {}

        void log(const std::string& msg, const std::string& level);

        std::ofstream targetFileStream;
    };
}
