/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"
#include "math/SoloVector2.h"
#include "math/SoloVector3.h"

namespace solo
{
    class Texture2D;
    class Device;

    // As class because script binding doesn't like structs
    class GlyphInfo
    {
    public:
        vec<Vector3> positions;
        vec<Vector2> uvs;
        float offsetX, offsetY;
    };

    class Font
    {
    public:
        static auto loadFromFile(Device *device, const str &path,
                                 u32 size, u32 atlasWidth, u32 atlasHeight,
                                 u32 firstChar, u32 charCount,
                                 u32 oversampleX, u32 oversampleY) -> sptr<Font>;

        Font(const Font &other) = delete;
        Font(Font &&other) = delete;
        virtual ~Font() = default;

        auto operator=(const Font &other) -> Font & = delete;
        auto operator=(Font &&other) -> Font & = delete;

        virtual auto atlas() const -> sptr<Texture2D> = 0;
        virtual auto glyphInfo(u32 character, float offsetX, float offsetY) -> GlyphInfo = 0;

    protected:
        Font() = default;
    };
}