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
        class NullFrameBuffer : public FrameBuffer
        {
        public:
            void setAttachments(const vec<sptr<Texture2d>> &attachments) override final {}
        };
    }
}