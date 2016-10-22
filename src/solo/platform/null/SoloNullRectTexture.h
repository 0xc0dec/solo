#pragma once

#include "SoloRectTexture.h"


namespace solo
{
    class NullRectTexture: public RectTexture
    {
    public:
        void bind() override final {}
        void generateMipmaps() override final {}
        void setData(TextureFormat format, const uint8_t* data, uint32_t width, uint32_t height) override final {}
    };
}