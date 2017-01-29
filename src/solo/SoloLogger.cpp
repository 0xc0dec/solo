/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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
        SL_ERR_IF(!impl->file.is_open(), SL_FMT("Unable to open target log file ", path));
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
