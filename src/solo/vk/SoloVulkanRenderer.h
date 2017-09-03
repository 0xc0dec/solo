/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

// NOTE This file, as well as other vulkan files in the engine, is a total hackfest and test playground currently.
// Don't pay attention to the quality of code here.

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloRenderer.h"
#include "SoloVulkanSwapchain.h"
#include "SoloVulkanPipeline.h"
#include "SoloRenderCommand.h"
#include "SoloVulkan.h"

namespace solo
{
    class Device;
    class Camera;

    namespace vk
    {
        class Mesh;

        class Renderer final : public solo::Renderer
        {
        public:
            explicit Renderer(Device *device);
            ~Renderer();

            // TODO avoid these?
            auto getDevice() const -> VkDevice { return device; }
            auto getPhysicalDevice() const -> VkPhysicalDevice { return physicalDevice; }
            auto getPhysicalFeatures() const -> VkPhysicalDeviceFeatures { return physicalFeatures; }
            auto getPhysicalProperties() const -> VkPhysicalDeviceProperties { return physicalProperties; }
            auto getPhysicalMemoryFeatures() const -> VkPhysicalDeviceMemoryProperties { return physicalMemoryFeatures; }
            auto getColorFormat() const -> VkFormat { return colorFormat; }
            auto getDepthFormat() const -> VkFormat { return depthFormat; }
            auto getColorSpace() const -> VkColorSpaceKHR { return colorSpace; }
            auto getCommandPool() const -> VkCommandPool { return commandPool; }
            auto getQueue() const -> VkQueue { return queue; }

            void addRenderCommand(const RenderCommand &cmd) override final { renderCommands.push_back(cmd); }

        protected:
            void beginFrame() override final;
            void endFrame() override final;

        private:
            Device *engineDevice = nullptr;

            Resource<VkDevice> device;
            Resource<VkCommandPool> commandPool;
            VkPhysicalDevice physicalDevice = nullptr;
            VkPhysicalDeviceFeatures physicalFeatures{};
            VkPhysicalDeviceProperties physicalProperties{};
            VkPhysicalDeviceMemoryProperties physicalMemoryFeatures{};
            VkFormat colorFormat = VK_FORMAT_UNDEFINED;
            VkFormat depthFormat = VK_FORMAT_UNDEFINED;
            VkColorSpaceKHR colorSpace = VK_COLOR_SPACE_MAX_ENUM_KHR;
            VkQueue queue = nullptr;
            Resource<VkDebugReportCallbackEXT> debugCallback;
            Swapchain swapchain;

            std::vector<RenderCommand> renderCommands;
            std::vector<Pipeline> pipelines;
        };
    }
}

#endif
