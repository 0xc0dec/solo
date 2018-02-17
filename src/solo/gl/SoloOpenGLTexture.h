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
    class OpenGLTexture: public NoCopyAndMove
    {
    public:
        OpenGLTexture();
        virtual ~OpenGLTexture();

        auto getHandle() const -> GLuint { return handle; }

        virtual void bind() = 0;

    protected:
        GLuint handle = 0;
    };

    class OpenGLTexture2D final: public Texture2D, public OpenGLTexture
    {
    public:
        explicit OpenGLTexture2D(Texture2DData *data, bool generateMipmaps);

        void bind() override final;

    private:
        void setData(const void *data, bool generateMipmaps);
    };

    class OpenGLCubeTexture final : public CubeTexture, public OpenGLTexture
    {
    public:
        explicit OpenGLCubeTexture(CubeTextureData *data, bool generateMipmaps);

        void bind() override final;

    protected:
        void setData(CubeTextureData *data, bool generateMipmaps);
    };
}

#endif