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

        enum class RenderCommandType
        {
            None,
            BeginCamera,
            EndCamera,
            DrawMesh
        };

        struct RenderCommand
        {
            RenderCommandType type = RenderCommandType::None;

            union
            {
                const Camera *camera;
                const Mesh *mesh;
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
        };
    }
}

#endif