/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloTexture.h"

namespace solo
{
    class NullTexture2D final: public Texture2D
    {
    public:
        explicit NullTexture2D(TextureFormat format, Vector2 dimensions): Texture2D(format, dimensions) {}
    };

    class NullCubeTexture final : public CubeTexture
    {
    public:
        explicit NullCubeTexture(TextureFormat format, u32 dimension): CubeTexture(format, dimension) {}
    };
}