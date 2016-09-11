/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"
#include "SoloVector3.h"


namespace solo
{
    class RectTexture;

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

        SL_DISABLE_COPY_AND_MOVE(Font)
        virtual ~Font() {}

        auto getAtlas() const -> sptr<RectTexture>;

        virtual auto getGlyphInfo(uint32_t character, float offsetX, float offsetY) -> GlyphInfo = 0;

    protected:
        Font() {}

        sptr<RectTexture> atlas;
    };

    inline auto Font::getAtlas() const -> sptr<RectTexture>
    {
        return atlas;
    }
}