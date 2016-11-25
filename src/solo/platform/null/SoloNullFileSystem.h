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

#include "SoloFileSystem.h"


namespace solo
{
    class NullFileSystem final: public FileSystem
    {
    public:
        auto readBytes(const std::string &path) -> std::vector<uint8_t> override final;
        void writeBytes(const std::string &path, const std::vector<uint8_t> &data) override final;
        auto readText(const std::string &path) -> std::string override final;
        auto readLines(const std::string &path) -> std::vector<std::string> override final;
        void iterateLines(const std::string &path, std::function<bool(const std::string &)> process) override final;
        void writeLines(const std::string &path, const std::vector<std::string> &lines) override final;
    };

    inline auto NullFileSystem::readBytes(const std::string &path) -> std::vector<uint8_t>
    {
        return {};
    }

    inline void NullFileSystem::writeBytes(const std::string &path, const std::vector<uint8_t> &data)
    {
    }

    inline auto NullFileSystem::readText(const std::string &path) -> std::string
    {
        return "";
    }

    inline auto NullFileSystem::readLines(const std::string &path) -> std::vector<std::string>
    {
        return {};
    }

    inline void NullFileSystem::iterateLines(const std::string &path, std::function<bool(const std::string &)> process)
    {
    }

    inline void NullFileSystem::writeLines(const std::string &path, const std::vector<std::string> &lines)
    {
    }
}