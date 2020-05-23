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

        void beginCamera(Camera *camera) override;
        void endCamera(Camera *camera) override;
        void drawMesh(Mesh *mesh, Transform *transform, Material *material) override;
        void drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material) override;

        auto name() const -> const char* override { return name_.c_str(); }
        auto gpuName() const -> const char* override;

    protected:
        void beginFrame() override;
        void endFrame() override;

    private:
        str name_;
        Camera *currentCamera_ = nullptr;
    };
}

#endif
