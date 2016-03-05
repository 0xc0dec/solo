#pragma once

#include "SoloRenderer.h"

namespace solo
{
    class StubRenderer: public Renderer
    {
    public:
        virtual TextureHandle createTexture() override { return EmptyTextureHandle; }
        virtual void destroyTexture(TextureHandle handle) override {}

        virtual void set2DTexture(TextureHandle handle) override {}
        virtual void set2DTexture(TextureHandle handle, TextureFlags flags) override {}
        virtual void set2DTexture(TextureHandle handle, TextureFlags flags, float anisotropyLevel) override {}

        virtual void setCubeTexture(TextureHandle handle) override {}
        virtual void setCubeTexture(TextureHandle handle, TextureFlags flags) override {}
        virtual void setCubeTexture(TextureHandle handle, TextureFlags flags, float anisotropyLevel) override {}

        virtual void update2DTexture(TextureHandle handle, ColorFormat format, int width, int height, const std::vector<uint8_t>& data) override {}
        virtual void updateCubeTexture(TextureHandle handle, CubeTextureFace face, ColorFormat format, int width, int height, const std::vector<uint8_t>& data) override {}

    private:
        friend class Renderer;

        StubRenderer() {}
    };
}