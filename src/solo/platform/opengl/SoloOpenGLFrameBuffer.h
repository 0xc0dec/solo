#pragma once

#include "SoloFrameBuffer.h"
#include "SoloOpenGLRenderer.h"

namespace solo
{
    class Device;

    class OpenGLFrameBuffer final: public FrameBuffer
    {
    public:
        explicit OpenGLFrameBuffer(Device* device);
        ~OpenGLFrameBuffer();

        void bind() override final;
        void unbind() override final;

        void setAttachments(const std::vector<sptr<RectTexture>>& attachments) override final;

    private:
        OpenGLRenderer* renderer = nullptr;
        FrameBufferHandle handle = EmptyFrameBufferHandle;
    };
}