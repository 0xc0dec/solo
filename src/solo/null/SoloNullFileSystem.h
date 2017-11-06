/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloFileSystem.h"

namespace solo
{
    namespace null
    {
        class FileSystem final : public solo::FileSystem
        {
        public:
            auto readBytes(const str &path) -> std::vector<uint8_t> override final;
            void writeBytes(const str &path, const std::vector<uint8_t> &data) override final;
            auto readText(const str &path) -> str override final;
            auto readLines(const str &path) -> std::vector<str> override final;
            void iterateLines(const str &path, std::function<bool(const str &)> process) override final;
            void writeLines(const str &path, const std::vector<str> &lines) override final;
        };

        inline auto FileSystem::readBytes(const str &path) -> std::vector<uint8_t>
        {
            return {};
        }

        inline void FileSystem::writeBytes(const str &path, const std::vector<uint8_t> &data)
        {
        }

        inline auto FileSystem::readText(const str &path) -> str
        {
            return "";
        }

        inline auto FileSystem::readLines(const str &path) -> std::vector<str>
        {
            return {};
        }

        inline void FileSystem::iterateLines(const str &path, std::function<bool(const str &)> process)
        {
        }

        inline void FileSystem::writeLines(const str &path, const std::vector<str> &lines)
        {
        }
    }
}