/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

namespace solo
{
    struct VulkanPrefabShaders
    {
        struct Vertex
        {
            static const s8 *skybox;
            static const s8 *font;
        };

        struct Fragment
        {
            static const s8 *skybox;
            static const s8 *font;
        };
    };
}

#endif
