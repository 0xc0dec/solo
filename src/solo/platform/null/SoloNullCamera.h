#pragma once

#include "SoloCamera.h"

namespace solo
{
    class NullCamera final: public Camera
    {
    public:
        explicit NullCamera(const Node& node): Camera(node)
        {
        }
    };
}