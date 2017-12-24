/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloTexture.h"

namespace solo
{
    class NullTexture2d final: public Texture2d
    {
    public:
        explicit NullTexture2d(Texture2dData *data): Texture2d(data) {}
    };

    class NullCubeTexture final : public CubeTexture
    {
    public:
        explicit NullCubeTexture(CubeTextureData *data): CubeTexture(data) {}
    };
}