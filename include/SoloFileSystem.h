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

#pragma once

#include "SoloCommon.h"
#include <vector>
#include <functional>


namespace solo
{
    class Device;
    class DeviceToken;

    class FileSystem
    {
    public:
        static auto create(Device* device, const DeviceToken&) -> sptr<FileSystem>;

        virtual ~FileSystem() {}
        SL_NONCOPYABLE(FileSystem)

        virtual auto readBytes(const std::string& path) -> std::vector<uint8_t>;
        virtual void writeBytes(const std::string& path, const std::vector<uint8_t>& data);

        virtual auto readText(const std::string& path) -> std::string;
        virtual auto readLines(const std::string& path) -> std::vector<std::string>;
        virtual void iterateLines(const std::string& path, std::function<bool(const std::string&)> process);
        virtual void writeLines(const std::string& path, const std::vector<std::string>& lines);

    protected:
        FileSystem() {}
    };
}
