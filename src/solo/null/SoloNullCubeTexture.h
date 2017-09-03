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
            CubeTexture(uint32_t width, uint32_t height, uint32_t depth, TextureFormat format):
                solo::CubeTexture(width, height, depth, format)
            {
            }

            void generateMipmaps() override final {}

        protected:
            void setData(CubeTextureData *data) override final {}
        };
    }
}