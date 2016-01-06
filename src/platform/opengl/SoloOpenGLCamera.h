#pragma once

#include "SoloCamera.h"
#include "SoloNode.h"

namespace solo
{
    class Device;
    class Scene;

    class OpenGLCamera: public Camera
    {
    public:
        virtual void init() override;

    private:
        friend class Camera;

        OpenGLCamera(Scene *scene, Node node);

        Device *device = nullptr;

        void applyViewport() override;
        void applyClearColor() override;
        void clear() override;
    };
}