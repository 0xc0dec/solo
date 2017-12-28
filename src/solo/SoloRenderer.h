/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
    struct RenderCommand;

    class Renderer
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Renderer)

        static auto create(Device *device, const FriendToken<Device> &) -> sptr<Renderer>;

        virtual ~Renderer() {}

        virtual void beginCamera(Camera *camera, FrameBuffer *renderTarget) = 0;
        virtual void endCamera(Camera *camera, FrameBuffer *renderTarget) = 0;
        virtual void drawMesh(Mesh *mesh, Transform *transform, Material *material) = 0;
        virtual void drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material) = 0;

        void renderFrame(std::function<void()> render);

    protected:
        Renderer() {}

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
    };
}