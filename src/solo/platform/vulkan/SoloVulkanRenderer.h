#pragma once

#include "SoloRenderer.h"

#ifdef SL_VULKAN_RENDERER

namespace solo
{
    class VulkanRenderer final: public Renderer
    {
    public:
        void beginFrame() override final;
        void endFrame() override final;

    private:
    };
}

#else
#   error Vulkan renderer is not supported on this platform
#endif
