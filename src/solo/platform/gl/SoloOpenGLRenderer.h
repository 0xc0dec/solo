/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloRenderer.h"
#include "SoloRenderCommand.h"
#include <vector>
#include <functional>

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
            struct RenderStep
            {
                RenderCommand cmd;
                std::function<void()> beginCamera;
                std::function<void()> endCamera;
                std::function<void()> applyMaterialState;
            };

            std::vector<RenderStep> renderSteps;
        };
    }
}

#endif
