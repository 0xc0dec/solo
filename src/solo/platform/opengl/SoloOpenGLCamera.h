#pragma once

#include "SoloCamera.h"

namespace solo
{
    class OpenGLCamera: public Camera
    {
    public:
        explicit OpenGLCamera(const Node& node);

    private:
    };
}