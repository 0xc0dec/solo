/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

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

        void setTargetFile(const str &path);

        void logDebug(const str &msg);
        void logInfo(const str &msg);
        void logWarning(const str &msg);
        void logError(const str &msg);
        void logCritical(const str &msg);

    private:
        sptr<LoggerImpl> impl; // TODO remove
    };
}
