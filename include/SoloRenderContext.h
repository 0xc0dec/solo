#pragma once

namespace solo
{
    struct RenderContext
    {
        class Camera *camera;
        class Transform *cameraTransform;
        class Transform *nodeTransform;
        class Scene *scene = nullptr;
    };
}