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
        void beginCamera(Camera *camera, FrameBuffer *renderTarget) override final {}
        void endCamera(Camera *camera, FrameBuffer *renderTarget) override final {}
        void drawMesh(Mesh *mesh, Transform *transform, Material *material) override final {}
        void drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material) override final {}

    protected:
        void beginFrame() override final {}
        void endFrame() override final {}
    };
}