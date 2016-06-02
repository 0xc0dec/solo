#pragma once

#include "SoloFileSystem.h"


namespace solo
{
    class StubFileSystem: public FileSystem
    {
    public:
        auto readBytes(const std::string& path) -> std::vector<uint8_t> override;
        void writeBytes(const std::string& path, const std::vector<uint8_t>& data) override;

        auto readText(const std::string& path) -> std::string override;

        auto readLines(const std::string& path) -> std::vector<std::string> override;
        void iterateLines(const std::string& path, std::function<bool(const std::string&)> process) override;
        void writeLines(const std::string& path, const std::vector<std::string>& lines) override;
    };
}