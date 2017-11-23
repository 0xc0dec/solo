/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloFileSystem.h"

namespace solo
{
    class NullFileSystem final : public FileSystem
    {
    public:
        auto readBytes(const str &path) -> vec<u8> override final { return {}; }
        void writeBytes(const str &path, const vec<u8> &data) override final {}
        auto readText(const str &path) -> str override final { return ""; }
        auto readLines(const str &path) -> vec<str> override final { return {}; }
        void iterateLines(const str &path, std::function<bool(const str &)> process) override final {}
        void writeLines(const str &path, const vec<str> &lines) override final {}
    };
}