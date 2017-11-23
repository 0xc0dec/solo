/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloSTBTrueTypeFont.h"
#include "SoloTexture.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloStringUtils.h"
#include "SoloTextureData.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

using namespace solo;

stb::STBTrueTypeFont::STBTrueTypeFont(Device *device, u8 *fontData, u32 size, u32 atlasWidth, u32 atlasHeight,
    u32 firstChar, u32 charCount, u32 oversampleX, u32 oversampleY):
    firstChar(firstChar)
{
    charInfo = std::make_unique<stbtt_packedchar[]>(charCount);

    vec<u8> pixels;
    pixels.resize(atlasWidth * atlasHeight);

    stbtt_pack_context context;
    auto ret = stbtt_PackBegin(&context, pixels.data(), atlasWidth, atlasHeight, 0, 1, nullptr);
    SL_PANIC_IF(!ret)

    stbtt_PackSetOversampling(&context, oversampleX, oversampleY);
    stbtt_PackFontRange(&context, fontData, 0, static_cast<float>(size), firstChar, charCount, charInfo.get());
    stbtt_PackEnd(&context);

    auto data = Texture2dData::createFromMemory(atlasWidth, atlasHeight, TextureFormat::Red, pixels);
    atlas = Texture2d::createFromData(device, data.get());
    atlas->setFiltering(TextureFiltering::Linear);
    atlas->generateMipmaps();
}

auto stb::STBTrueTypeFont::getGlyphInfo(u32 character, float offsetX, float offsetY) -> GlyphInfo
{
    stbtt_aligned_quad quad;
    const auto atlasSize = atlas->getDimensions();

    stbtt_GetPackedQuad(charInfo.get(), static_cast<u32>(atlasSize.x), static_cast<u32>(atlasSize.y),
    character - firstChar, &offsetX, &offsetY, &quad, 1);
    auto xmin = quad.x0;
    auto xmax = quad.x1;
    auto ymin = -quad.y1;
    auto ymax = -quad.y0;

    auto result = GlyphInfo();
    result.offsetX = offsetX;
    result.offsetY = offsetY;
    result.positions =
    {
        { xmin, ymin, 0 },
        { xmin, ymax, 0 },
        { xmax, ymax, 0 },
        { xmax, ymin, 0 }
    };
    result.uvs =
    {
        { quad.s0, quad.t1 },
        { quad.s0, quad.t0 },
        { quad.s1, quad.t0 },
        { quad.s1, quad.t1 }
    };

    return result; // TODO move
}

bool stb::STBTrueTypeFont::canLoadFromFile(const str &path)
{
    return stringutils::endsWith(path, ".ttf");
}

auto stb::STBTrueTypeFont::loadFromFile(Device *device, const str &path,
    u32 size, u32 atlasWidth,
    u32 atlasHeight, u32 firstChar, u32 charCount, u32 oversampleX,
    u32 oversampleY) -> sptr<STBTrueTypeFont>
{
    auto data = device->getFileSystem()->readBytes(path);
    return std::make_shared<STBTrueTypeFont>(device, data.data(), size, atlasWidth, atlasHeight, firstChar, charCount, oversampleX, oversampleY);
}
