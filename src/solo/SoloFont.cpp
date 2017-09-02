/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloFont.h"
#include "stb/SoloStbTrueTypeFont.h"

using namespace solo;

auto Font::loadFromFile(Device *device, const std::string &path,
    uint32_t size, uint32_t atlasWidth,
    uint32_t atlasHeight, uint32_t firstChar, uint32_t charCount,
    uint32_t oversampleX, uint32_t oversampleY) -> sptr<Font>
{
    if (stb::TrueTypeFont::canLoadFromFile(path))
        return stb::TrueTypeFont::loadFromFile(device, path, size, atlasWidth, atlasHeight, firstChar, charCount, oversampleX, oversampleY);
    SL_PANIC("Unsupported font file ", path);
    return nullptr;
}
