/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCamera.h"

namespace solo
{
    namespace null
    {
        class Camera final : public solo::Camera
        {
        public:
            explicit Camera(const Node &node) : solo::Camera(node)
            {
            }
        };
    }
}