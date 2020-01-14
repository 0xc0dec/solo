/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"
#include <functional>

namespace solo
{
    class Device;
    class Camera;
    class FrameBuffer;
    class Mesh;
    class Transform;
    class Material;

    class Renderer: public NoCopyAndMove
    {
    public:
        static auto fromDevice(Device *device) -> sptr<Renderer>;

        virtual ~Renderer() = default;

        virtual void beginCamera(Camera *camera, FrameBuffer *renderTarget) = 0;
        virtual void endCamera(Camera *camera, FrameBuffer *renderTarget) = 0;
        virtual void drawMesh(Mesh *mesh, Transform *transform, Material *material) = 0;
        virtual void drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material) = 0;

        virtual auto name() const -> const char* = 0;
        virtual auto gpuName() const -> const char* = 0;

        void renderFrame(std::function<void()> render);

    protected:
        Renderer() = default;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
    };
}