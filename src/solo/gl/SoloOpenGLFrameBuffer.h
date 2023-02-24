/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloFrameBuffer.h"
#include "SoloOpenGL.h"

namespace solo {
    class Device;
    class OpenGLTexture2D;

    class OpenGLFrameBuffer final : public FrameBuffer {
    public:
        static auto fromAttachments(const vec<sptr<Texture2D>> &attachments) -> sptr<OpenGLFrameBuffer>;

        ~OpenGLFrameBuffer();

        auto handle() const -> GLuint {
            return handle_;
        }

    private:
        GLuint handle_ = 0;
        vec<sptr<OpenGLTexture2D>> colorAttachments_;
        sptr<OpenGLTexture2D> depthAttachment_;

        OpenGLFrameBuffer() = default;
    };
}

#endif
