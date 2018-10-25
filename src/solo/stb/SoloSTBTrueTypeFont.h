/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloFont.h"
#include <stb_truetype.h>

namespace solo
{
    class STBTrueTypeFont final : public Font
    {
    public:
        static bool canLoadFromFile(const str &path);
        static auto loadFromFile(Device *device, const str &path,
            u32 size, u32 atlasWidth, u32 atlasHeight,
            u32 firstChar, u32 charCount,
            u32 oversampleX, u32 oversampleY) -> sptr<STBTrueTypeFont>;

        auto getAtlas() const -> sptr<Texture2D> override final { return atlas_; }
        auto getGlyphInfo(u32 character, float offsetX, float offsetY) -> GlyphInfo override final;

    private:
        sptr<Texture2D> atlas_;
        u32 firstChar_ = 0;
        uptr<stbtt_packedchar[]> charInfo_;

        STBTrueTypeFont() = default;
    };
}