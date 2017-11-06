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

    namespace gl
    {
        class Texture2d;

        class FrameBuffer final : public solo::FrameBuffer
        {
        public:
            FrameBuffer();
            ~FrameBuffer();

            auto getHandle() const -> GLuint { return handle; }

            void setAttachments(const vec<sptr<solo::Texture2d>> &attachments) override final;

        private:
            GLuint handle = 0;
            GLuint depthBufferHandle = 0;
            u32 attachmentCount = 0;
        };
    }
}

#endif
