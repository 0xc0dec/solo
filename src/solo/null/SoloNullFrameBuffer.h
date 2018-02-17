/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloFrameBuffer.h"

namespace solo
{
    class NullFrameBuffer : public FrameBuffer
    {
    public:
        void setAttachments(const vec<sptr<Texture2D>> &attachments) override final {}
    };
}