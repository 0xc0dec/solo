/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloFrameBuffer.h"

namespace solo
{
    namespace null
    {
        class FrameBuffer : public solo::FrameBuffer
        {
        public:
            void setAttachments(const std::vector<sptr<RectTexture>> &attachments) override final {}
        };
    }
}