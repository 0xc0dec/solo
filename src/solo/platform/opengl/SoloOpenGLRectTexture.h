#pragma once

#include "SoloRectTexture.h"


namespace solo
{
    class OpenGLRenderer;
    class Device;

    class OpenGLRectTexture: public RectTexture
    {
    public:
        explicit OpenGLRectTexture(Device* device);

        void bind() override final;
        void generateMipmaps() override final;
        void setData(TextureFormat format, const uint8_t* data, uint32_t width, uint32_t height) override final;

    private:
        OpenGLRenderer* renderer = nullptr;
        TextureHandle handle = EmptyTextureHandle;
    };
}