/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloFont.h"
#include "stb/SoloStbTrueTypeFont.h"

using namespace solo;

auto Font::loadFromFile(Device *device, const str &path,
    u32 size, u32 atlasWidth,
    u32 atlasHeight, u32 firstChar, u32 charCount,
    u32 oversampleX, u32 oversampleY) -> sptr<Font>
{
    if (stb::TrueTypeFont::canLoadFromFile(path))
        return stb::TrueTypeFont::loadFromFile(device, path, size, atlasWidth, atlasHeight, firstChar, charCount, oversampleX, oversampleY);
    SL_PANIC(SL_FMT("Unsupported font file ", path));
    return nullptr;
}
