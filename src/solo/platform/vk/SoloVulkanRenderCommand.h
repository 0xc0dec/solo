/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

namespace solo
{
    namespace vk
    {
        class Camera;
        class Mesh;
        class Material;

        enum class RenderCommandType
        {
            None,
            BeginCamera,
            EndCamera,
            DrawMesh,
            ApplyMaterial
        };

        struct RenderCommand
        {
            RenderCommandType type = RenderCommandType::None;

            union
            {
                const Camera *camera;
                const Mesh *mesh;
                const Material *material;
            };

            RenderCommand(RenderCommandType type = RenderCommandType::None): type(type) {}

            static auto beginCamera(const Camera *camera) -> RenderCommand
            {
                auto cmd = RenderCommand(RenderCommandType::BeginCamera);
                cmd.camera = camera;
                return cmd;
            }

            static auto endCamera() -> RenderCommand
            {
                auto cmd = RenderCommand(RenderCommandType::EndCamera);
                return cmd;
            }

            static auto drawMesh(const Mesh *mesh) -> RenderCommand
            {
                auto cmd = RenderCommand(RenderCommandType::DrawMesh);
                cmd.mesh = mesh;
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
}

#endif