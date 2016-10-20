#pragma once

#include "SoloCamera.h"

namespace solo
{
    class OpenGLCamera final: public Camera
    {
    public:
        explicit OpenGLCamera(const Node& node);

    protected:
        void applyViewport(float x, float y, float width, float height) const override final;
        void applyDepthState(bool test, bool write) const override final;
        void clear(bool color, bool depth, float r, float g, float b, float a) const override final;
    };
}