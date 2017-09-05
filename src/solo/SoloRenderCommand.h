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
                Mesh *mesh;
                Transform *transform;
                uint32_t part;
            } meshPart;

            struct
            {
                Mesh *mesh;
                Transform *transform;
            } mesh;

            Camera *camera;
            Material *material;
        };

        explicit RenderCommand(RenderCommandType type = RenderCommandType::None): type(type) {}

        static auto beginCamera(Camera *camera) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::BeginCamera);
            cmd.camera = camera;
            return cmd;
        }

        static auto endCamera(Camera *camera) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::EndCamera);
            cmd.camera = camera;
            return cmd;
        }

        static auto drawMesh(Mesh *mesh, Transform *transform) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::DrawMesh);
            cmd.mesh.mesh = mesh;
            cmd.mesh.transform = transform;
            return cmd;
        }

        static auto drawMeshPart(Mesh *mesh, uint32_t part, Transform *transform) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::DrawMeshPart);
            cmd.meshPart.mesh = mesh;
            cmd.meshPart.part = part;
            cmd.meshPart.transform = transform;
            return cmd;
        }

        static auto applyMaterial(Material *material) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::ApplyMaterial);
            cmd.material = material;
            return cmd;
        }
    };
}