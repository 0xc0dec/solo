/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloFileSystem.h"
#include "SoloDevice.h"
#include "null/SoloNullFileSystem.h"
#include <fstream>

using namespace solo;

auto FileSystem::create(Device *device, const FriendToken<Device> &) -> sptr<FileSystem>
{
    if (device->getSetup().mode == DeviceMode::Null)
        return std::make_shared<null::FileSystem>();
    return std::unique_ptr<FileSystem>(new FileSystem());
}

auto FileSystem::getStream(const std::string &path) -> sptr<std::istream>
{
    std::ifstream file{path};
    SL_PANIC_IF(!file.is_open());
    return std::make_shared<std::ifstream>(std::move(file));
}

auto FileSystem::readBytes(const std::string &path) -> std::vector<uint8_t>
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    SL_PANIC_IF(!file.is_open(), SL_FMT("Failed to open file '", path, "'"));

    const auto size = file.tellg();
    file.seekg(0, std::ios::beg);
    auto result = std::vector<uint8_t>(size);
    file.read(reinterpret_cast<char *>(&result[0]), size);
    file.close();

    return result;
}

void FileSystem::writeBytes(const std::string &path, const std::vector<uint8_t> &data)
{
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    SL_PANIC_IF(!file.is_open(), SL_FMT("Failed to open file '", path, "'"));
    file.write(reinterpret_cast<const char *>(&data[0]), data.size());
    file.close();
}

auto FileSystem::readText(const std::string &path) -> std::string
{
    std::ifstream f(path);
    auto result = std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
    return result;
}

auto FileSystem::readLines(const std::string &path) -> std::vector<std::string>
{
    std::vector<std::string> result;
    iterateLines(path, [&](const std::string & s)
    {
        result.push_back(s);
        return true;
    });
    return result;
}

void FileSystem::iterateLines(const std::string &path, std::function<bool(const std::string &)> process)
{
    std::ifstream file(path);
    SL_PANIC_IF(!file.is_open(), SL_FMT("Failed to open file '", path, "'"));
    while (!file.eof())
    {
        std::string line;
        std::getline(file, line);
        if (!process(line))
            break;
    }
    file.close();
}

void FileSystem::writeLines(const std::string &path, const std::vector<std::string> &lines)
{
    std::ofstream file(path, std::ios::trunc);
    SL_PANIC_IF(!file.is_open(), SL_FMT("Failed to open file '", path, "'"));
    for (size_t i = 0; i < lines.size(); ++i)
    {
        file << lines[i];
        if (i < lines.size() - 1)
            file << "\n";
    }
    file.close();
}
