#pragma once

#include "SoloBase.h"
#include "SoloSpinLock.h"
#include <fstream>


namespace solo
{
    class DeviceToken;

    class Logger final
    {
    public:
        explicit Logger(const DeviceToken& t) {}
        ~Logger();
        SL_NONCOPYABLE(Logger)

        void setTargetFile(const std::string& path);

        void logDebug(const std::string& msg);
        void logInfo(const std::string& msg);
        void logWarning(const std::string& msg);
        void logError(const std::string& msg);
        void logCritical(const std::string& msg);

    private:
        void log(const std::string& msg, const std::string& level);

        std::ofstream file;
        SpinLock lock;
    };
}
