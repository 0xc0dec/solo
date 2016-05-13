#pragma once

#include "SoloFileSystem.h"


namespace solo
{
    class StubFileSystem: public FileSystem
    {
    public:
        virtual auto readBytes(const std::string& path) ->std::vector<uint8_t> override;
        virtual void writeBytes(const std::string& path, const std::vector<uint8_t>& data) override;

        virtual auto readText(const std::string& path) -> std::string override;

        virtual auto readLines(const std::string& path) -> std::vector<std::string> override;
        virtual void iterateLines(const std::string& path, std::function<bool(const std::string&)> process) override;
        virtual void writeLines(const std::string& path, const std::vector<std::string>& lines) override;
    };
}