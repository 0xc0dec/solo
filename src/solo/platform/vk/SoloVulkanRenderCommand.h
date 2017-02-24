/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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

            RenderCommand() {}

            static auto beginCamera(const Camera *camera) -> RenderCommand
            {
                auto cmd = RenderCommand();
                cmd.type = RenderCommandType::BeginCamera;
                cmd.camera = camera;
                return cmd;
            }

            static auto endCamera() -> RenderCommand
            {
                auto cmd = RenderCommand();
                cmd.type = RenderCommandType::EndCamera;
                return cmd;
            }

            static auto drawMesh(const Mesh *mesh) -> RenderCommand
            {
                auto cmd = RenderCommand();
                cmd.type = RenderCommandType::DrawMesh;
                cmd.mesh = mesh;
                return cmd;
            }
        };
    }
}

#endif