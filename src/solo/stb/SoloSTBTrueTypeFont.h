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
        class STBTrueTypeFont final : public Font
        {
        public:
            static bool canLoadFromFile(const str &path);
            static auto loadFromFile(Device *device, const str &path,
                u32 size, u32 atlasWidth, u32 atlasHeight,
                u32 firstChar, u32 charCount,
                u32 oversampleX, u32 oversampleY) -> sptr<STBTrueTypeFont>;

            STBTrueTypeFont(Device *device, u8 *fontData,
                u32 size, u32 atlasWidth, u32 atlasHeight,
                u32 firstChar, u32 charCount,
                u32 oversampleX, u32 oversampleY);

            auto getAtlas() const -> sptr<Texture2d> override final { return atlas; }
            auto getGlyphInfo(u32 character, float offsetX, float offsetY) -> GlyphInfo override final;

        private:
            sptr<Texture2d> atlas;
            u32 firstChar;
            uptr<stbtt_packedchar[]> charInfo;
        };
    }
}