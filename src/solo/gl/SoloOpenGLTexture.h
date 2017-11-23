/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGL.h"
#include "SoloTexture.h"

namespace solo
{
    class OpenGLTexture
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(OpenGLTexture)

        OpenGLTexture();
        virtual ~OpenGLTexture();

        auto getHandle() const -> GLuint { return handle; }

        virtual void bind() = 0;

    protected:
        GLuint handle = 0;
    };

    class OpenGLTexture2d final: public Texture2d, public OpenGLTexture
    {
    public:
        OpenGLTexture2d(Texture2dData *data);

        void bind() override final;
        void generateMipmaps() override final;

    private:
        void setData(const void *data);
    };

    class OpenGLCubeTexture final : public CubeTexture, public OpenGLTexture
    {
    public:
        OpenGLCubeTexture(CubeTextureData *data);

        void bind() override final;
        void generateMipmaps() override final;

    protected:
        void setData(CubeTextureData *data);
    };
}

#endif