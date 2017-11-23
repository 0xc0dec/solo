/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloTexture.h"

namespace solo
{
    namespace null
    {
        class NullTexture2d final: public Texture2d
        {
        public:
            NullTexture2d(Texture2dData *data): Texture2d(data) {}

            void generateMipmaps() override final {}
        };

        class NullCubeTexture final : public CubeTexture
        {
        public:
            NullCubeTexture(CubeTextureData *data): CubeTexture(data) {}
                
            void generateMipmaps() override final {}
        };
    }
}