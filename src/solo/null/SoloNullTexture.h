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
        explicit NullTexture2D(Texture2DData *data): Texture2D(data) {}
    };

    class NullCubeTexture final : public CubeTexture
    {
    public:
        explicit NullCubeTexture(CubeTextureData *data): CubeTexture(data) {}
    };
}