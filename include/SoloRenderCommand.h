/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

namespace solo
{
    class Mesh;
    class Transform;
    class Camera;
    class FrameBuffer;
    class Material;

    enum class RenderCommandType
    {
        None,
        BeginCamera,
        EndCamera,
        DrawMesh,
        DrawMeshPart,
        ApplyMaterial
    };

    struct RenderCommand
    {
        RenderCommandType type = RenderCommandType::None;

        union
        {
            struct
            {
                const Mesh *mesh;
                const Transform *transform;
                uint32_t part;
            } meshPart;

            struct
            {
                const Camera *camera;
                const FrameBuffer *frameBuffer;
            } camera;

            struct
            {
                const Mesh *mesh;
                const Transform *transform;
            } mesh;
            
            const Material *material;
        };

        explicit RenderCommand(RenderCommandType type = RenderCommandType::None): type(type) {}

        static auto beginCamera(const Camera *camera, const FrameBuffer *frameBuffer) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::BeginCamera);
            cmd.camera.camera = camera;
            cmd.camera.frameBuffer = frameBuffer;
            return cmd;
        }

        static auto endCamera() -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::EndCamera);
            return cmd;
        }

        static auto drawMesh(const Mesh *mesh, const Transform *transform) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::DrawMesh);
            cmd.mesh.mesh = mesh;
            cmd.mesh.transform = transform;
            return cmd;
        }

        static auto drawMeshPart(const Mesh *mesh, uint32_t part, const Transform *transform) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::DrawMeshPart);
            cmd.meshPart.mesh = mesh;
            cmd.meshPart.part = part;
            cmd.meshPart.transform = transform;
            return cmd;
        }

        static auto applyMaterial(const Material *material) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::ApplyMaterial);
            cmd.material = material;
            return cmd;
        }
    };
}