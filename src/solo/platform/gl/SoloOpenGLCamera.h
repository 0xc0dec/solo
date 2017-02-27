/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloCamera.h"

namespace solo
{
    namespace gl
    {
        class Renderer;

        class Camera final : public solo::Camera
        {
        public:
            explicit Camera(const Node &node);

        private:
            Renderer *renderer = nullptr;
        };
    }
}

#endif
