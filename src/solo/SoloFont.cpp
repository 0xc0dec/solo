/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloFont.h"
#include "stb/SoloSTBTrueTypeFont.h"

using namespace solo;

auto Font::loadFromFile(Device *device, const str &path,
    u32 size, u32 atlasWidth,
    u32 atlasHeight, u32 firstChar, u32 charCount,
    u32 oversampleX, u32 oversampleY) -> sptr<Font>
{
    if (STBTrueTypeFont::canLoadFromFile(path))
        return STBTrueTypeFont::loadFromFile(device, path, size, atlasWidth, atlasHeight, firstChar, charCount, oversampleX, oversampleY);
    debugPanicIf(true, "Unsupported font file ", path);
    return nullptr;
}
