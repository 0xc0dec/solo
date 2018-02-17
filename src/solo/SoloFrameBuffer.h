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
        static auto create(Device *device) -> sptr<FrameBuffer>;

		virtual ~FrameBuffer() = default;

        virtual void setAttachments(const vec<sptr<Texture2D>> &attachments) = 0;

        auto getDimensions() const -> Vector2 { return dimensions; }

    protected:
        Vector2 dimensions;

		FrameBuffer() = default;
    };
}
