/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloFileSystem.h"
#include "SoloDevice.h"
#include <fstream>

using namespace solo;

auto FileSystem::fromDevice(Device *device) -> sptr<FileSystem> {
    return std::unique_ptr<FileSystem>(new FileSystem());
}

auto FileSystem::stream(const str &path) -> sptr<std::istream> {
    std::ifstream file{path};
    panicIf(!file.is_open(), "Unable to open read stream for file ", path);
    return std::make_shared<std::ifstream>(std::move(file));
}

auto FileSystem::readBytes(const str &path) -> vec<u8> {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    panicIf(!file.is_open(), "Unable to open file ", path);

    const auto size = file.tellg();
    file.seekg(0, std::ios::beg);
    auto result = vec<u8>(size);
    file.read(reinterpret_cast<s8 *>(&result[0]), size);
    file.close();

    return result;
}

void FileSystem::writeBytes(const str &path, const vec<u8> &data) {
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    panicIf(!file.is_open(), "Unable to open file ", path);
    file.write(reinterpret_cast<const s8 *>(&data[0]), data.size());
    file.close();
}

auto FileSystem::readText(const str &path) -> str {
    std::ifstream f(path);
    auto result = str(std::istreambuf_iterator<s8>(f), std::istreambuf_iterator<s8>());
    return result;
}

auto FileSystem::readLines(const str &path) -> vec<str> {
    vec<str> result;
    iterateLines(path, [&](const str & s) {
        result.push_back(s);
        return true;
    });
    return result;
}

void FileSystem::iterateLines(const str &path, std::function<bool(const str &)> process) {
    std::ifstream file(path);
    panicIf(!file.is_open(), "Unable to open file ", path);
    while (!file.eof()) {
        str line;
        std::getline(file, line);
        if (!process(line))
            break;
    }
    file.close();
}

void FileSystem::writeLines(const str &path, const vec<str> &lines) {
    std::ofstream file(path, std::ios::trunc);
    panicIf(!file.is_open(), "Unable to open file ", path);
    for (size_t i = 0; i < lines.size(); ++i) {
        file << lines[i];
        if (i < lines.size() - 1)
            file << "\n";
    }
    file.close();
}
