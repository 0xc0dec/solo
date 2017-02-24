/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloFrameBuffer.h"
#include "SoloOpenGLRenderer.h"

namespace solo
{
    class Device;

    namespace gl
    {
        class FrameBuffer final : public solo::FrameBuffer
        {
        public:
            explicit FrameBuffer(Device *device);
            ~FrameBuffer();

            void bind() override final;
            void unbind() override final;

            void setAttachments(const std::vector<sptr<solo::RectTexture>> &attachments) override final;

        private:
            Renderer *renderer = nullptr;
            uint32_t handle = EmptyHandle;
        };
    }
}

#endif
