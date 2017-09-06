/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"
#include <vector>

namespace solo
{
    class Device;
    class Texture2d;

    class FrameBuffer
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(FrameBuffer)

        static auto create(Device *device) -> sptr<FrameBuffer>;

        virtual ~FrameBuffer() {}

        virtual void setAttachments(const std::vector<sptr<Texture2d>> &attachments) = 0;

        auto getDimensions() const -> Vector2 { return dimensions; }

    protected:
        Vector2 dimensions;

        FrameBuffer() {}
    };
}
