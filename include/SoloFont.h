#pragma once

#include "SoloBase.h"
#include "SoloVector2.h"
#include "SoloVector3.h"


namespace solo
{
    class Texture2D;

    struct GlyphInfo
    {
        Vector3 positions[4];
        Vector2 uvs[4];
        float offsetX, offsetY;
    };

    class Font
    {
    public:
        static auto create(uint8_t* fontData, uint32_t size, uint32_t atlasWidth, uint32_t atlasHeight,
            uint32_t firstChar, uint32_t charCount, uint32_t oversampleX, uint32_t oversampleY) -> sptr<Font>;

        virtual ~Font() {}
        SL_NONCOPYABLE(Font)

        auto getAtlas() const -> sptr<Texture2D>;

        virtual auto getGlyphInfo(uint32_t character, float offsetX, float offsetY) -> GlyphInfo = 0;

    protected:
        Font() {}

        sptr<Texture2D> atlas;
    };

    inline auto Font::getAtlas() const -> sptr<Texture2D>
    {
        return atlas;
    }
}