#pragma once

#include "SoloFont.h"
#include <stb_truetype.h>


namespace solo
{
    class TrueTypeFont final: public Font
    {
    public:
        virtual auto getGlyphInfo(uint32_t character, float offsetX, float offsetY) -> GlyphInfo override final;

    private:
        friend class Font;

        TrueTypeFont(uint8_t* fontData, uint32_t size, uint32_t atlasWidth, uint32_t atlasHeight, uint32_t firstChar, uint32_t charCount);

        uint32_t firstChar;
        uptr<stbtt_packedchar[]> charInfo;
    };
}