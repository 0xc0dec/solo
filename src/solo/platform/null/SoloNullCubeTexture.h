#pragma once

#include "SoloCubeTexture.h"


namespace solo
{
    class NullCubeTexture final: public CubeTexture
    {
    public:
        NullCubeTexture() {}

        void bind() override final {}
        void generateMipmaps() override final {}
        void setData(CubeTextureFace face, TextureFormat format, const uint8_t* data, uint32_t width, uint32_t height) override final {}
    };
}