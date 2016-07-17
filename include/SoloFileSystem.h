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
