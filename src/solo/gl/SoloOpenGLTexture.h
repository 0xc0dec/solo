/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
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
        OpenGLTexture();
        OpenGLTexture(const OpenGLTexture &other) = delete;
        OpenGLTexture(OpenGLTexture &&other) = delete;
        virtual ~OpenGLTexture();

        auto operator=(const OpenGLTexture &other) -> OpenGLTexture & = delete;
        auto operator=(OpenGLTexture &&other) -> OpenGLTexture & = delete;

        auto handle() const -> GLuint
        {
            return handle_;
        }

        virtual void bind() = 0;

    protected:
        GLuint handle_ = 0;
    };

    class OpenGLTexture2D final: public Texture2D, public OpenGLTexture
    {
    public:
        static auto empty(u32 width, u32 height, TextureFormat format) -> sptr<OpenGLTexture2D>;
        static auto fromData(sptr<Texture2DData> data, bool generateMipmaps) -> sptr<OpenGLTexture2D>;

        void bind() override;

    private:
        OpenGLTexture2D(TextureFormat format, Vector2 dimensions);
    };

    class OpenGLCubeTexture final : public CubeTexture, public OpenGLTexture
    {
    public:
        static auto fromData(sptr<CubeTextureData> data) -> sptr<OpenGLCubeTexture>;

        void bind() override;

    private:
        OpenGLCubeTexture(TextureFormat format, u32 dimension);
    };
}

#endif