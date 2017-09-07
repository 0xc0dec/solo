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
        class Texture2d final: public solo::Texture2d
        {
        public:
            Texture2d(Texture2dData *data): solo::Texture2d(data) {}

            void generateMipmaps() override final {}
        };

        class CubeTexture final : public solo::CubeTexture
        {
        public:
            CubeTexture(CubeTextureData *data): solo::CubeTexture(data) {}
                
            void generateMipmaps() override final {}
        };
    }
}