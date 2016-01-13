#pragma once

#include "SoloTexture2D.h"
#include <GL/glew.h>

namespace solo
{
    class OpenGLTexture2D: public Texture2D
    {
    public:
        virtual ~OpenGLTexture2D();

        virtual void apply() override;
        virtual void generateMipmaps() override;

        GLuint getHandle() const;

    protected:
        virtual void applyData(ColorFormat format, const std::vector<uint8_t>& data, unsigned width, unsigned height) override;

    private:
        friend class Texture;

        OpenGLTexture2D();

        void bind();
        void unbind();

        GLuint handle = 0;
    };
}