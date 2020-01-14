/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloRenderer.h"

namespace solo
{
    class Device;

    class OpenGLRenderer final : public Renderer
    {
    public:
        explicit OpenGLRenderer(Device *device);
        ~OpenGLRenderer() = default;

        void beginCamera(Camera *camera, FrameBuffer *renderTarget) override final;
        void endCamera(Camera *camera, FrameBuffer *renderTarget) override final;
        void drawMesh(Mesh *mesh, Transform *transform, Material *material) override final;
        void drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material) override final;

        auto name() const -> const char* override final { return name_.c_str(); }
        auto gpuName() const -> const char* override final;

    protected:
        void beginFrame() override final;
        void endFrame() override final;

    private:
        str name_;
        Camera *currentCamera_ = nullptr;

        void applyMaterial(Material *material);
    };
}

#endif
