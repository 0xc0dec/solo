#include "SoloTrueTypeFont.h"
#include "SoloTexture2D.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

using namespace solo;


TrueTypeFont::TrueTypeFont(uint8_t* fontData, uint32_t size, uint32_t atlasWidth, uint32_t atlasHeight, uint32_t firstChar, uint32_t charCount):
    firstChar(firstChar)
{
    charInfo = std::make_unique<stbtt_packedchar[]>(charCount);
    
    auto pixels = std::make_unique<uint8_t[]>(atlasWidth * atlasHeight);

    stbtt_pack_context context;
    auto ret = stbtt_PackBegin(&context, pixels.get(), atlasWidth, atlasHeight, 0, 1, nullptr);
    SL_DEBUG_THROW_IF(!ret, InvalidOperationException, "Failed to initialize font");

    stbtt_PackSetOversampling(&context, 2, 2); // TODO parameters
    stbtt_PackFontRange(&context, fontData, 0, size, firstChar, charCount, charInfo.get());
    stbtt_PackEnd(&context);

    atlas = Texture2D::create();
    atlas->setFiltering(TextureFiltering::Linear);
    atlas->setData(TextureFormat::Red, pixels.get(), atlasWidth, atlasHeight);
    atlas->generateMipmaps();
}


auto TrueTypeFont::getGlyphInfo(uint32_t character, float offsetX, float offsetY) -> GlyphInfo
{
    stbtt_aligned_quad quad;
    auto atlasSize = atlas->getSize();

    stbtt_GetPackedQuad(charInfo.get(), atlasSize.x, atlasSize.y, character - firstChar, &offsetX, &offsetY, &quad, 1);
    auto xmin = quad.x0;
    auto xmax = quad.x1;
    auto ymin = -quad.y1;
    auto ymax = -quad.y0;

    auto result = GlyphInfo();
    result.offsetX = offsetX;
    result.offsetY = offsetY;
    result.position[0] = Vector3(xmin, ymin, 0);
    result.position[1] = Vector3(xmin, ymax, 0);
    result.position[2] = Vector3(xmax, ymax, 0);
    result.position[3] = Vector3(xmax, ymin, 0);
    result.uv[0] = Vector2(quad.s0, quad.t1);
    result.uv[1] = Vector2(quad.s0, quad.t0);
    result.uv[2] = Vector2(quad.s1, quad.t0);
    result.uv[3] = Vector2(quad.s1, quad.t1);
    return result; // TODO move
}


auto Font::create(uint8_t* fontData, uint32_t size, uint32_t atlasWidth, uint32_t atlasHeight, uint32_t firstChar, uint32_t charCount) -> sptr<Font>
{
    // TODO if constructors throws...
    return std::unique_ptr<Font>(new TrueTypeFont(fontData, size, atlasWidth, atlasHeight, firstChar, charCount));
}
