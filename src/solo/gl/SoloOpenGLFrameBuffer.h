/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloFrameBuffer.h"
#include "SoloOpenGL.h"

namespace solo
{
    class Device;
    class OpenGLTexture2D;

    class OpenGLFrameBuffer final : public FrameBuffer
    {
    public:
		static auto create(const vec<sptr<Texture2D>> &attachments) -> sptr<OpenGLFrameBuffer>;

        ~OpenGLFrameBuffer();

        auto getHandle() const -> GLuint { return handle; }

    private:
        GLuint handle = 0;
        vec<sptr<OpenGLTexture2D>> colorAttachments;
		sptr<OpenGLTexture2D> depthAttachment;

		OpenGLFrameBuffer() = default;
    };
}

#endif
