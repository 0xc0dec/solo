#pragma once

#include "SoloCubeTexture.h"
#include "SoloOpenGLRenderer.h"


namespace solo
{
    class OpenGLCubeTexture final: public CubeTexture
    {
    public:
        explicit OpenGLCubeTexture(Device* device);
        ~OpenGLCubeTexture();

        void bind() override final;
        void generateMipmaps() override final;

        void setData(CubeTextureFace face, TextureFormat format, const uint8_t* data, uint32_t width, uint32_t height) override final;

    private:
        OpenGLRenderer* renderer = nullptr;
        TextureHandle handle = EmptyTextureHandle;
    };
}