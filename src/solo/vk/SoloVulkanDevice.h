/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"

namespace solo
{
	// Rename to VulkanDriverDevice to free up name for VulkanDevice impl of the engine Device
    class VulkanDevice
    {
    public:
        VulkanDevice() = default;
        VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
        VulkanDevice(VulkanDevice &&other) = default;
        VulkanDevice(const VulkanDevice &other) = delete;
        ~VulkanDevice() = default;

        bool isFormatSupported(VkFormat format, VkFormatFeatureFlags features) const;
        auto gpuName() const -> const char *{ return physicalProperties_.deviceName; }

        auto operator=(const VulkanDevice &other) -> VulkanDevice& = delete;
        auto operator=(VulkanDevice &&other) -> VulkanDevice& = default;

        operator VkDevice() const { return handle_; }

        auto handle() const -> VkDevice { return handle_; }
        auto surface() const -> VkSurfaceKHR { return surface_; }
        auto physical() const -> VkPhysicalDevice { return physical_; }
        auto physicalFeatures() const -> VkPhysicalDeviceFeatures { return physicalFeatures_; }
        auto physicalProperties() const -> VkPhysicalDeviceProperties { return physicalProperties_; }
        auto physicalMemoryFeatures() const -> VkPhysicalDeviceMemoryProperties { return physicalMemoryFeatures_; }
        auto colorFormat() const -> VkFormat { return colorFormat_; }
        auto depthFormat() const -> VkFormat { return depthFormat_; }
        auto colorSpace() const -> VkColorSpaceKHR { return colorSpace_; }
        auto commandPool() const -> VkCommandPool { return commandPool_; }
        auto queue() const -> VkQueue { return queue_; }
    	auto queueIndex() const -> u32 { return queueIndex_; }

    private:
        VulkanResource<VkDevice> handle_;
        VkSurfaceKHR surface_ = nullptr;
        VulkanResource<VkCommandPool> commandPool_;
        VkPhysicalDevice physical_ = nullptr;
        VkPhysicalDeviceFeatures physicalFeatures_{};
        VkPhysicalDeviceProperties physicalProperties_{};
        VkPhysicalDeviceMemoryProperties physicalMemoryFeatures_{};
        VkFormat colorFormat_ = VK_FORMAT_UNDEFINED;
        VkFormat depthFormat_ = VK_FORMAT_UNDEFINED;
        VkColorSpaceKHR colorSpace_ = VK_COLOR_SPACE_MAX_ENUM_KHR;
        VkQueue queue_ = nullptr;
    	u32 queueIndex_ = -1;
        VulkanResource<VkDebugReportCallbackEXT> debugCallback_;
        umap<VkFormat, VkFormatFeatureFlags> supportedFormats_;

        void selectPhysicalDevice(VkInstance instance);
        void detectFormatSupport(VkFormat format);
        auto selectDepthFormat() const -> VkFormat;
    };
}

#endif