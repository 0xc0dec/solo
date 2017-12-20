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
                u32 part;
                Material *material;
            } meshPart;

            struct
            {
                Mesh *mesh;
                Transform *transform;
                Material *material;
            } mesh;

            struct
            {
                Camera *camera;
                FrameBuffer *renderTarget;
            } camera;
        };

        explicit RenderCommand(RenderCommandType type = RenderCommandType::None): type(type) {}

        static auto beginCamera(Camera *camera, FrameBuffer *renderTarget) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::BeginCamera);
            cmd.camera.camera = camera;
            cmd.camera.renderTarget = renderTarget;
            return cmd;
        }

        static auto endCamera(Camera *camera) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::EndCamera);
            cmd.camera.camera = camera;
            cmd.camera.renderTarget = nullptr; // TODO is this correct?
            return cmd;
        }

        static auto drawMesh(Mesh *mesh, Transform *transform, Material *material) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::DrawMesh);
            cmd.mesh.mesh = mesh;
            cmd.mesh.transform = transform;
            cmd.mesh.material = material;
            return cmd;
        }

        static auto drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material) -> RenderCommand
        {
            auto cmd = RenderCommand(RenderCommandType::DrawMeshPart);
            cmd.meshPart.mesh = mesh;
            cmd.meshPart.part = part;
            cmd.meshPart.transform = transform;
            cmd.meshPart.material = material;
            return cmd;
        }
    };
}