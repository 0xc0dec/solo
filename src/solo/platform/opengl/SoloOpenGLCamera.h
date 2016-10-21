#pragma once

#include "SoloCamera.h"

namespace solo
{
    class OpenGLRenderer;

    class OpenGLCamera final: public Camera
    {
    public:
        explicit OpenGLCamera(const Node& node);

    private:
        void applyImpl() const override final;

        OpenGLRenderer* renderer = nullptr;
    };
}