#pragma once

namespace solo
{
    struct RenderContext
    {
        class Camera* camera = nullptr;
        class Transform* cameraTransform = nullptr;
        class Transform* nodeTransform = nullptr;
        class Scene* scene = nullptr;

        static RenderContext empty;
    };
}