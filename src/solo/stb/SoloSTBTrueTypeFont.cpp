/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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

auto STBTrueTypeFont::glyphInfo(u32 character, float offsetX, float offsetY) -> GlyphInfo
{
    const auto dimensions = atlas_->dimensions();

    stbtt_aligned_quad quad;
    stbtt_GetPackedQuad(charInfo_.get(), static_cast<u32>(dimensions.x()), static_cast<u32>(dimensions.y()),
        character - firstChar_, &offsetX, &offsetY, &quad, 1);

    auto xmin = quad.x0;
    auto xmax = quad.x1;
    auto ymin = -quad.y1;
    auto ymax = -quad.y0;

    GlyphInfo result{};
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

bool STBTrueTypeFont::canLoadFromFile(const str &path)
{
    return stringutils::endsWith(path, ".ttf");
}

auto STBTrueTypeFont::loadFromFile(Device *device, const str &path, u32 size, u32 atlasWidth, u32 atlasHeight,
    u32 firstChar, u32 charCount, u32 oversampleX, u32 oversampleY) -> sptr<STBTrueTypeFont>
{
    auto data = device->fileSystem()->readBytes(path);

    auto result = sptr<STBTrueTypeFont>(new STBTrueTypeFont());
    result->firstChar_ = firstChar;
    result->charInfo_ = std::make_unique<stbtt_packedchar[]>(charCount);

    vec<u8> pixels;
    pixels.resize(atlasWidth * atlasHeight);

    stbtt_pack_context context;
    const auto ret = stbtt_PackBegin(&context, pixels.data(), atlasWidth, atlasHeight, 0, 1, nullptr);
    SL_DEBUG_PANIC(!ret, "Unable to process font ", path);

    stbtt_PackSetOversampling(&context, oversampleX, oversampleY);
    stbtt_PackFontRange(&context, data.data(), 0, static_cast<float>(size), firstChar, charCount, result->charInfo_.get());
    stbtt_PackEnd(&context);

    const auto atlasData = Texture2DData::fromMemory(atlasWidth, atlasHeight, TextureDataFormat::Red, pixels);
    result->atlas_ = Texture2D::fromData(device, atlasData, true);
    result->atlas_->setFilter(TextureFilter::Linear, TextureFilter::Linear, TextureMipFilter::Linear);

    return result;
}
