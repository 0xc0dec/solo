#pragma once

#include "SoloBase.h"
#include "SoloRenderer.h"
#include "SoloResourcePool.h"
#include <GL/glew.h>


namespace solo
{
    class OpenGLRenderer final: public Renderer
    {
    public:
        ~OpenGLRenderer();

        virtual TextureHandle createTexture() override final;
        virtual void destroyTexture(TextureHandle handle) override final;

        virtual void set2DTexture(TextureHandle handle) override final;
        virtual void set2DTexture(TextureHandle handle, int flags) override final;
        virtual void set2DTexture(TextureHandle handle, int flags, float anisotropyLevel) override final;

        virtual void setCubeTexture(TextureHandle handle) override final;
        virtual void setCubeTexture(TextureHandle handle, int flags) override final;
        virtual void setCubeTexture(TextureHandle handle, int flags, float anisotropyLevel) override final;
        
        virtual void update2DTexture(TextureHandle handle, ColorFormat format, int width, int height,
            const std::vector<uint8_t>& data) override final;
        virtual void updateCubeTexture(TextureHandle handle, CubeTextureFace face, ColorFormat format, int width, int height,
            const std::vector<uint8_t>& data) override final;

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

        ResourcePool<TextureData, SL_MAX_TEXTURES> textures;
        ResourcePool<FrameBufferData, SL_MAX_FRAME_BUFFERS> frameBuffers;
    };
}