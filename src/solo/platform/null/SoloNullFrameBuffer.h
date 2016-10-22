#pragma once

#include "SoloFrameBuffer.h"

namespace solo
{
    class NullFrameBuffer: public FrameBuffer
    {
    public:
        void bind() override final {}
        void unbind() override final {}
        void setAttachments(const std::vector<sptr<RectTexture>>& attachments) override final {}
    };
}