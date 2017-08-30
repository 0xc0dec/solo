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
            auto readBytes(const std::string &path) -> std::vector<uint8_t> override final;
            void writeBytes(const std::string &path, const std::vector<uint8_t> &data) override final;
            auto readText(const std::string &path) -> std::string override final;
            auto readLines(const std::string &path) -> std::vector<std::string> override final;
            void iterateLines(const std::string &path, std::function<bool(const std::string &)> process) override final;
            void writeLines(const std::string &path, const std::vector<std::string> &lines) override final;
        };

        inline auto FileSystem::readBytes(const std::string &path) -> std::vector<uint8_t>
        {
            return {};
        }

        inline void FileSystem::writeBytes(const std::string &path, const std::vector<uint8_t> &data)
        {
        }

        inline auto FileSystem::readText(const std::string &path) -> std::string
        {
            return "";
        }

        inline auto FileSystem::readLines(const std::string &path) -> std::vector<std::string>
        {
            return {};
        }

        inline void FileSystem::iterateLines(const std::string &path, std::function<bool(const std::string &)> process)
        {
        }

        inline void FileSystem::writeLines(const std::string &path, const std::vector<std::string> &lines)
        {
        }
    }
}