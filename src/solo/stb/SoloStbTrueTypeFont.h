/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloFont.h"
#include <stb_truetype.h>

namespace solo
{
    namespace stb
    {
        class TrueTypeFont final : public Font
        {
        public:
            static bool canLoadFromFile(const str &path);
            static auto loadFromFile(Device *device, const str &path,
                uint32_t size, uint32_t atlasWidth, uint32_t atlasHeight,
                uint32_t firstChar, uint32_t charCount,
                uint32_t oversampleX, uint32_t oversampleY) -> sptr<TrueTypeFont>;

            TrueTypeFont(Device *device, uint8_t *fontData,
                uint32_t size, uint32_t atlasWidth, uint32_t atlasHeight,
                uint32_t firstChar, uint32_t charCount,
                uint32_t oversampleX, uint32_t oversampleY);

            auto getAtlas() const -> sptr<Texture2d> override final { return atlas; }
            auto getGlyphInfo(uint32_t character, float offsetX, float offsetY) -> GlyphInfo override final;

        private:
            sptr<Texture2d> atlas;
            uint32_t firstChar;
            uptr<stbtt_packedchar[]> charInfo;
        };
    }
}