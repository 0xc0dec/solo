/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloRenderer.h"
#include <vector>

namespace solo
{
    class Device;

    namespace gl
    {
        class Renderer final : public solo::Renderer
        {
        public:
            explicit Renderer(Device *device);
            ~Renderer() {}

            void addRenderCommand(const RenderCommand &cmd) override final;

        protected:
            void beginFrame() override final;
            void endFrame() override final;

        private:
            std::vector<RenderCommand> renderCommands;
        };
    }
}

#endif
