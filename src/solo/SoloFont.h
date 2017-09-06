/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include <vector>

namespace solo
{
    class Texture2d;
    class Device;

    // class because script binding doesn't like structs
    class GlyphInfo
    {
    public:
        std::vector<Vector3> positions;
        std::vector<Vector2> uvs;
        float offsetX, offsetY;
    };

    class Font
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Font)

        static auto loadFromFile(Device *device, const std::string &path,
            uint32_t size, uint32_t atlasWidth, uint32_t atlasHeight,
            uint32_t firstChar, uint32_t charCount,
            uint32_t oversampleX, uint32_t oversampleY) -> sptr<Font>;

        virtual ~Font() {}

        virtual auto getAtlas() const -> sptr<Texture2d> = 0;
        virtual auto getGlyphInfo(uint32_t character, float offsetX, float offsetY) -> GlyphInfo = 0;

    protected:
        Font() {}
    };
}