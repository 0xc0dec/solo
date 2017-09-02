/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <functional>

namespace solo
{
    class Device;
    struct RenderCommand;

    class Renderer
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Renderer)

        static auto create(Device *device, const FriendToken<Device> &) -> sptr<Renderer>;

        virtual ~Renderer() {}

        void renderFrame(std::function<void()> render);

        virtual void addRenderCommand(const RenderCommand &cmd) = 0;

    protected:
        Renderer() {}

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
    };
}