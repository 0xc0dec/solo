#pragma once

#include "SoloCamera.h"

namespace solo
{
    class StubCamera: public Camera
    {
    protected:
        virtual void applyViewport() override {}
        virtual void applyClearColor() override {}
        virtual void clear() override {}

    private:
        friend class Camera;

        StubCamera(Scene* scene, const Node& node):
            Camera(scene, node)
        {
        }
    };
}