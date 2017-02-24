/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloRenderer.h"

namespace solo
{
    namespace null
    {
        class Renderer final : public solo::Renderer
        {
        protected:
            void beginFrame() override final {}
            void endFrame() override final {}
        };
    }
}