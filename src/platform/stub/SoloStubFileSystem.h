#pragma once

#include "SoloFileSystem.h"

namespace solo
{
    class StubFileSystem: public FileSystem
    {
    public:
        virtual std::vector<uint8_t> readBytes(const std::string &path) override;
        virtual void writeBytes(const std::string &path, const std::vector<uint8_t> &data) override;

        virtual std::string readText(const std::string &path) override;

        virtual std::vector<std::string> readLines(const std::string &path) override;
        virtual void iterateLines(const std::string &path, std::function<bool(const std::string &)> process) override;
        virtual void writeLines(const std::string &path, const std::vector<std::string> &lines) override;

    private:
        friend class FileSystem;

        StubFileSystem() {}
    };
}