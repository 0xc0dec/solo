/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloFrameBuffer.h"
#include <GL/glew.h>

namespace solo
{
    class Device;

    namespace gl
    {
        class RectTexture;

        class FrameBuffer final : public solo::FrameBuffer
        {
        public:
            FrameBuffer();
            ~FrameBuffer();

            void bind() const;
            void unbind() const;

            void setAttachments(const std::vector<sptr<solo::RectTexture>> &attachments) override final;

        private:
            GLuint handle = 0;
            GLuint depthBufferHandle = 0;
            uint32_t attachmentCount = 0;
        };
    }
}

#endif
