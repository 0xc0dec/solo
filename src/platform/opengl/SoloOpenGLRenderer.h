#pragma once

#include "SoloRenderer.h"
#include "../../SoloResourcePool.h"
#include <GL/glew.h>

namespace solo
{
    const int MaxTextureCount = 65535;

    class OpenGLRenderer: public Renderer
    {
    public:
        ~OpenGLRenderer();

        virtual TextureHandle createTexture() override;
        virtual void destroyTexture(TextureHandle handle) override;

        virtual void set2DTexture(TextureHandle handle) override;
        virtual void set2DTexture(TextureHandle handle, TextureFlags flags) override;
        virtual void set2DTexture(TextureHandle handle, TextureFlags flags, float anisotropyLevel) override;

        virtual void setCubeTexture(TextureHandle handle) override;
        virtual void setCubeTexture(TextureHandle handle, TextureFlags flags) override;
        virtual void setCubeTexture(TextureHandle handle, TextureFlags flags, float anisotropyLevel) override;
        
        virtual void update2DTexture(TextureHandle handle, ColorFormat format, int width, int height, const std::vector<uint8_t>& data) override;
        virtual void updateCubeTexture(TextureHandle handle, CubeTextureFace face, ColorFormat format, int width, int height, const std::vector<uint8_t>& data) override;

    private:
        friend class Renderer;

        OpenGLRenderer();

        void bindTexture(GLenum target, TextureHandle handle);
        void setTexture(GLenum target, TextureHandle handle, TextureFlags flags);

        ResourcePool<GLuint, MaxTextureCount> textureHandles;
    };
}