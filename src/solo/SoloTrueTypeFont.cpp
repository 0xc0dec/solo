/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTrueTypeFont.h"
#include "SoloRectTexture.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

using namespace solo;

TrueTypeFont::TrueTypeFont(Device *device, uint8_t *fontData, uint32_t size, uint32_t atlasWidth, uint32_t atlasHeight,
                           uint32_t firstChar, uint32_t charCount, uint32_t oversampleX, uint32_t oversampleY):
    firstChar(firstChar)
{
    charInfo = std::make_unique<stbtt_packedchar[]>(charCount);

    auto pixels = std::make_unique<uint8_t[]>(atlasWidth * atlasHeight);

    stbtt_pack_context context;
    auto ret = stbtt_PackBegin(&context, pixels.get(), atlasWidth, atlasHeight, 0, 1, nullptr);
    SL_PANIC_IF(!ret)

    stbtt_PackSetOversampling(&context, oversampleX, oversampleY);
    stbtt_PackFontRange(&context, fontData, 0, static_cast<float>(size), firstChar, charCount, charInfo.get());
    stbtt_PackEnd(&context);

    atlas = RectTexture::create(device);
    atlas->setFiltering(TextureFiltering::Linear);
    atlas->setData(TextureFormat::Red, pixels.get(), atlasWidth, atlasHeight);
    atlas->generateMipmaps();
}

auto TrueTypeFont::getGlyphInfo(uint32_t character, float offsetX, float offsetY) -> GlyphInfo
{
    stbtt_aligned_quad quad;
    auto atlasSize = atlas->getSize();

    stbtt_GetPackedQuad(charInfo.get(), static_cast<uint32_t>(atlasSize.x), static_cast<uint32_t>(atlasSize.y),
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

auto Font::create(Device *device, uint8_t *fontData, uint32_t size, uint32_t atlasWidth, uint32_t atlasHeight,
                  uint32_t firstChar, uint32_t charCount, uint32_t oversampleX, uint32_t oversampleY) -> sptr<Font>
{
    // TODO if constructors throws...
    return std::make_unique<TrueTypeFont>(device, fontData, size, atlasWidth, atlasHeight, firstChar, charCount, oversampleX, oversampleY);
}
