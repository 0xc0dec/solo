/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"
#include "math/SoloVector2.h"

namespace solo
{
    class Device;
    class Texture2D;

    class FrameBuffer
    {
    public:
        static auto fromAttachments(Device *device, const vec<sptr<Texture2D>> &attachments) -> sptr<FrameBuffer>;

        FrameBuffer(const FrameBuffer &other) = delete;
        FrameBuffer(FrameBuffer &&other) = delete;
        virtual ~FrameBuffer() = default;

        auto operator=(const FrameBuffer &other) -> FrameBuffer & = delete;
        auto operator=(FrameBuffer &&other) -> FrameBuffer & = delete;

        auto dimensions() const -> Vector2
        {
            return dimensions_;
        }

    protected:
        Vector2 dimensions_;

        FrameBuffer() = default;

        static void validateNewAttachments(const vec<sptr<Texture2D>> &attachments);
    };
}
