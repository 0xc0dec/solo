#pragma once

#include "SoloRenderer.h"

namespace solo
{
    class StubRenderer final: public Renderer
    {
    public:
        virtual TextureHandle createTexture() override final { return EmptyTextureHandle; }
        virtual void destroyTexture(TextureHandle handle) override {}

        virtual void set2DTexture(TextureHandle handle) override {}
        virtual void set2DTexture(TextureHandle handle, int flags) override {}
        virtual void set2DTexture(TextureHandle handle, int flags, float anisotropyLevel) override {}

        virtual void setCubeTexture(TextureHandle handle) override {}
        virtual void setCubeTexture(TextureHandle handle, int flags) override {}
        virtual void setCubeTexture(TextureHandle handle, int flags, float anisotropyLevel) override {}

        virtual void update2DTexture(TextureHandle handle, ColorFormat format, int width, int height, const std::vector<uint8_t>& data) override {}
        virtual void updateCubeTexture(TextureHandle handle, CubeTextureFace face, ColorFormat format, int width, int height, const std::vector<uint8_t>& data) override {}

        virtual FrameBufferHandle createFrameBuffer() override final { return EmptyFrameBufferHandle; }
        virtual void destroyFrameBuffer(FrameBufferHandle handle) override final {}

        virtual void setFrameBuffer(FrameBufferHandle handle) override final {}

        virtual void updateFrameBuffer(FrameBufferHandle handle, const std::vector<TextureHandle> attachments) override final {}

    private:
        friend class Renderer;

        StubRenderer() {}
    };
}