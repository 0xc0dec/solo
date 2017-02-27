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

            void setFaceCull(FaceCull face);
            void setPolygonMode(PolygonMode mode);
            void setBlend(bool enabled);
            void setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor);
            void setDepthWrite(bool enabled);
            void setDepthTest(bool enabled);
            void setDepthFunction(DepthFunction func);
            void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
            void clear(bool color, bool depth, float r, float g, float b, float a);

        protected:
            void beginFrame() override final;
            void endFrame() override final;

        private:
            std::vector<RenderCommand> renderCommands;
        };
    }
}

#endif
