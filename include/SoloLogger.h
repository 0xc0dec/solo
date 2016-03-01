#pragma once

#include "SoloBase.h"
#include <fstream>

namespace solo
{
    class Logger
    {
    public:
        Logger() {}
        ~Logger();

        SL_NONCOPYABLE(Logger);

        void setTargetFile(const std::string& path);

        void logDebug(const std::string& msg);
        void logInfo(const std::string& msg);
        void logWarning(const std::string& msg);
        void logError(const std::string& msg);
        void logCritical(const std::string& msg);

    private:
        void log(const std::string& msg, const std::string& level);

        std::ofstream targetFileStream;
    };
}
