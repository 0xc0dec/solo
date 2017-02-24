/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <fstream>


namespace solo
{
    class Device;
    struct LoggerImpl;

    class Logger final
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Logger)

        explicit Logger(const FriendToken<Device> &);
        ~Logger();

        void setTargetFile(const std::string &path);

        void logDebug(const std::string &msg);
        void logInfo(const std::string &msg);
        void logWarning(const std::string &msg);
        void logError(const std::string &msg);
        void logCritical(const std::string &msg);

    private:
        sptr<LoggerImpl> impl;
    };
}
