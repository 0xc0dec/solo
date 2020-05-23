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
    class Mesh;
    class Transform;
    class Material;

    class Renderer: public NoCopyAndMove
    {
    public:
        static auto fromDevice(Device *device) -> sptr<Renderer>;

        virtual ~Renderer() = default;

        virtual void beginCamera(Camera *camera) = 0;
        virtual void endCamera(Camera *camera) = 0;
        virtual void renderMesh(Mesh *mesh, Transform *transform, Material *material) = 0;
        virtual void renderMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material) = 0;

        virtual auto name() const -> const char* = 0;
        virtual auto gpuName() const -> const char* = 0;

        void renderFrame(const std::function<void()> &render);

    protected:
        Renderer() = default;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
    };
}