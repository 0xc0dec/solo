/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"
#include "SoloVector3.h"

namespace solo
{
    class Texture2D;
    class Device;

    // class because script binding doesn't like structs
    class GlyphInfo
    {
    public:
        vec<Vector3> positions;
        vec<Vector2> uvs;
        float offsetX, offsetY;
    };

    class Font: public NoCopyAndMove
    {
    public:
        static auto loadFromFile(Device *device, const str &path,
            u32 size, u32 atlasWidth, u32 atlasHeight,
            u32 firstChar, u32 charCount,
            u32 oversampleX, u32 oversampleY) -> sptr<Font>;

        virtual ~Font() = default;

        virtual auto getAtlas() const -> sptr<Texture2D> = 0;
        virtual auto getGlyphInfo(u32 character, float offsetX, float offsetY) -> GlyphInfo = 0;

    protected:
        Font() = default;
    };
}