/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloRenderer.h"

namespace solo
{
    class NullRenderer final : public Renderer
    {
    public:
        void addRenderCommand(const RenderCommand &cmd) override {}

    protected:
        void beginFrame() override final {}
        void endFrame() override final {}
    };
}