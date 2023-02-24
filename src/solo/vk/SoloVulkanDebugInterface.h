/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"
#include "SoloVulkanRenderPass.h"
#include "../sdl/SoloSDLDebugInterface.h"

namespace solo {
    class VulkanDevice;
    class VulkanRenderer;
    class Device;

    class VulkanDebugInterface final: public SDLDebugInterface {
    public:
        explicit VulkanDebugInterface(Device *device);
        ~VulkanDebugInterface();

        void renderInto(VkCommandBuffer targetCmdBuffer) const;

    private:
        VulkanDevice *device_;
        VulkanRenderer *renderer_;
        VulkanResource<VkDescriptorPool> descPool_;
    };
}

#endif