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
    class RectTexture;

    class FrameBuffer
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(FrameBuffer)

        static auto create(Device *device) -> sptr<FrameBuffer>;

        virtual ~FrameBuffer() {}

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void setAttachments(const std::vector<sptr<RectTexture>> &attachments) = 0;

        auto getSize() const -> Vector2;

    protected:
        Vector2 size;

        FrameBuffer() {}
    };

    inline auto FrameBuffer::getSize() const -> Vector2
    {
        return size;
    }
}
