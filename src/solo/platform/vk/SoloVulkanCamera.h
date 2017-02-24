/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloCamera.h"

namespace solo
{
    namespace vk
    {
        class Renderer;

        class Camera final : public solo::Camera
        {
        public:
            explicit Camera(const Node &node);

        private:
            Renderer *renderer = nullptr;

            void beginFrame() const override final;
            void endFrame() const override final;
        };
    }
}

#endif