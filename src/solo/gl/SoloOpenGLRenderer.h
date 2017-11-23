/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloRenderer.h"
#include "SoloRenderCommand.h"
#include <functional>

namespace solo
{
    class Device;

    namespace gl
    {
        class OpenGLRenderer final : public Renderer
        {
        public:
            explicit OpenGLRenderer(Device *device);
            ~OpenGLRenderer() {}

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

            // Note: maybe not the fastest, but convenient and good enough for now
            vec<RenderStep> renderSteps;
        };
    }
}

#endif
