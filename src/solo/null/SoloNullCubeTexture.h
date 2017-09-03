/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCubeTexture.h"

namespace solo
{
    namespace null
    {
        class CubeTexture final : public solo::CubeTexture
        {
        public:
            CubeTexture(uint32_t dimension, TextureFormat format):
                solo::CubeTexture(dimension, format)
            {
            }

            void generateMipmaps() override final {}

        protected:
            void setData(CubeTextureData *data) override final {}
        };
    }
}