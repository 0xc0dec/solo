/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <vector>
#include <functional>

namespace solo
{
    class Device;

    class FileSystem
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(FileSystem)

        static auto create(Device *device, const FriendToken<Device> &) -> sptr<FileSystem>;

        virtual ~FileSystem() {}

        virtual auto getStream(const str &path) -> sptr<std::istream>;

        virtual auto readBytes(const str &path) -> std::vector<uint8_t>;
        virtual void writeBytes(const str &path, const std::vector<uint8_t> &data);

        virtual auto readText(const str &path) -> str;
        virtual auto readLines(const str &path) -> std::vector<str>;
        virtual void iterateLines(const str &path, std::function<bool(const str &)> process);
        virtual void writeLines(const str &path, const std::vector<str> &lines);

    protected:
        FileSystem() {}
    };
}
