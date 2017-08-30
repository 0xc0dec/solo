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
            void generateMipmaps() override final {}
            void setData(CubeTextureFace face, TextureFormat format, const void *data, uint32_t width, uint32_t height) override final {}
        };
    }
}