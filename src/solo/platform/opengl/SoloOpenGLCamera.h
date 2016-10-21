#pragma once

#include "SoloCamera.h"

namespace solo
{
    class OpenGLRenderer;

    class OpenGLCamera: public Camera
    {
    public:
        explicit OpenGLCamera(const Node& node);

    private:
        void applyImpl() const override;

        OpenGLRenderer* renderer = nullptr;
    };
}