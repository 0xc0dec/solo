/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"
#include <functional>

namespace solo
{
    class Device;

    class FileSystem
    {
    public:
        static auto fromDevice(Device *device) -> sptr<FileSystem>;

        FileSystem(const FileSystem &other) = delete;
        FileSystem(FileSystem &&other) = delete;
        virtual ~FileSystem() = default;
    	
        auto operator=(const FileSystem &other) -> FileSystem& = delete;
        auto operator=(FileSystem &&other) -> FileSystem& = delete;

        virtual auto stream(const str &path) -> sptr<std::istream>;

        virtual auto readBytes(const str &path) -> vec<u8>;
        virtual void writeBytes(const str &path, const vec<u8> &data);

        virtual auto readText(const str &path) -> str;
        virtual auto readLines(const str &path) -> vec<str>;
        virtual void iterateLines(const str &path, std::function<bool(const str &)> process);
        virtual void writeLines(const str &path, const vec<str> &lines);

    protected:
        FileSystem() = default;
    };
}
