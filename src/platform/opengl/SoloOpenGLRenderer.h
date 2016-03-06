#pragma once

#include "SoloRenderer.h"
#include "../../SoloResourcePool.h"
#include <GL/glew.h>

namespace solo
{
    // TODO who knows...
    const int MaxTextureCount = 65535; // TODO as macroses in "Config.h"
    const int MaxFrameBufferCount = 1024;

    class OpenGLRenderer: public Renderer
    {
    public:
        ~OpenGLRenderer();

        virtual TextureHandle createTexture() override;
        virtual void destroyTexture(TextureHandle handle) override;

        virtual void set2DTexture(TextureHandle handle) override;
        virtual void set2DTexture(TextureHandle handle, int flags) override;
        virtual void set2DTexture(TextureHandle handle, int flags, float anisotropyLevel) override;

        virtual void setCubeTexture(TextureHandle handle) override;
        virtual void setCubeTexture(TextureHandle handle, int flags) override;
        virtual void setCubeTexture(TextureHandle handle, int flags, float anisotropyLevel) override;
        
        virtual void update2DTexture(TextureHandle handle, ColorFormat format, int width, int height,
            const std::vector<uint8_t>& data) override;
        virtual void updateCubeTexture(TextureHandle handle, CubeTextureFace face, ColorFormat format, int width, int height,
            const std::vector<uint8_t>& data) override;

        virtual FrameBufferHandle createFrameBuffer() override final;
        virtual void destroyFrameBuffer(FrameBufferHandle handle) override final;

        virtual void setFrameBuffer(FrameBufferHandle handle) override final;

        virtual void updateFrameBuffer(FrameBufferHandle handle, const std::vector<TextureHandle> attachments) override final;

    private:
        friend class Renderer;

        OpenGLRenderer();

        void bindFrameBuffer(FrameBufferHandle handle);
        void bindTexture(GLenum target, TextureHandle handle);
        void setTexture(GLenum target, TextureHandle handle, int flags);
        void validateFrameBufferAttachments(const std::vector<TextureHandle> attachments);

        struct FrameBufferData
        {
            GLuint rawHandle = 0;
            GLuint depthBufferHandle = 0;
            int attachmentCount = 0;
        };

        struct TextureData
        {
            GLuint rawHandle = 0;
            int width = 0;
            int height = 0;
        };

        ResourcePool<TextureData, MaxTextureCount> textures;
        ResourcePool<FrameBufferData, MaxFrameBufferCount> frameBuffers;
    };
}