/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"

namespace solo
{
    class Device;
    class Texture2D;

    class FrameBuffer: public NoCopyAndMove
    {
    public:
        static auto create(Device *device, const vec<sptr<Texture2D>> &attachments) -> sptr<FrameBuffer>;

        virtual ~FrameBuffer() = default;

        auto getDimensions() const -> Vector2 { return dimensions_; }

    protected:
        Vector2 dimensions_;

        FrameBuffer() = default;

        static void validateNewAttachments(const vec<sptr<Texture2D>> &attachments);
    };
}
