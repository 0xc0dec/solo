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
    if (device->getMode() == DeviceMode::Null)
        return std::make_shared<null::FileSystem>();
    return std::unique_ptr<FileSystem>(new FileSystem());
}

auto FileSystem::getStream(const str &path) -> sptr<std::istream>
{
    std::ifstream file{path};
    SL_PANIC_IF(!file.is_open());
    return std::make_shared<std::ifstream>(std::move(file));
}

auto FileSystem::readBytes(const str &path) -> std::vector<uint8_t>
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

void FileSystem::writeBytes(const str &path, const std::vector<uint8_t> &data)
{
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    SL_PANIC_IF(!file.is_open(), SL_FMT("Failed to open file '", path, "'"));
    file.write(reinterpret_cast<const char *>(&data[0]), data.size());
    file.close();
}

auto FileSystem::readText(const str &path) -> str
{
    std::ifstream f(path);
    auto result = str(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
    return result;
}

auto FileSystem::readLines(const str &path) -> std::vector<str>
{
    std::vector<str> result;
    iterateLines(path, [&](const str & s)
    {
        result.push_back(s);
        return true;
    });
    return result;
}

void FileSystem::iterateLines(const str &path, std::function<bool(const str &)> process)
{
    std::ifstream file(path);
    SL_PANIC_IF(!file.is_open(), SL_FMT("Failed to open file '", path, "'"));
    while (!file.eof())
    {
        str line;
        std::getline(file, line);
        if (!process(line))
            break;
    }
    file.close();
}

void FileSystem::writeLines(const str &path, const std::vector<str> &lines)
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
