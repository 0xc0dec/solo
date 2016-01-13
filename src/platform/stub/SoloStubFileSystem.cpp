#include "SoloStubFileSystem.h"

using namespace solo;


std::vector<uint8_t> StubFileSystem::readBytes(const std::string& path)
{
    return {};
}


void StubFileSystem::writeBytes(const std::string& path, const std::vector<uint8_t>& data)
{
}


std::string StubFileSystem::readText(const std::string& path)
{
    return "";
}


std::vector<std::string> StubFileSystem::readLines(const std::string& path)
{
    return {};
}


void StubFileSystem::iterateLines(const std::string& path, std::function<bool(const std::string&)> process)
{
}


void StubFileSystem::writeLines(const std::string& path, const std::vector<std::string>& lines)
{
}
