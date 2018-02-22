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
        static auto createFromData(sptr<Texture2DData> data, bool generateMipmaps) -> sptr<OpenGLTexture2D>;
        static auto createEmpty(u32 width, u32 height, TextureFormat format) -> sptr<OpenGLTexture2D>;

        void bind() override final;

    private:
        OpenGLTexture2D(TextureFormat format, Vector2 dimensions);
    };

    class OpenGLCubeTexture final : public CubeTexture, public OpenGLTexture
    {
    public:
        static auto createFromData(sptr<CubeTextureData> data) -> sptr<OpenGLCubeTexture>;

        void bind() override final;

    private:
        OpenGLCubeTexture(TextureFormat format, u32 dimension);
    };
}

#endif