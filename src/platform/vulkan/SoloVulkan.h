#pragma once

#include "SoloCommon.h"
#include <vector>


#ifdef SL_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR 
#endif
#include <vulkan.h>

#ifdef SL_ERR_CHECK
#   define SL_CHECK_VK_RESULT(vkCall, ...) SL_DBG_BLOCK(if (vkCall != VK_SUCCESS) SL_ERR(__VA_ARGS__))
#else
#   define SL_CHECK_VK_RESULT(vkCall) vkCall
#endif


namespace solo
{
    struct VulkanHelper
    {
        static auto createShader(VkDevice logicalDevice, const std::vector<uint8_t>& data) -> VkShaderModule;
        static auto createShaderStageInfo(bool vertex, VkShaderModule shader, const char* entryPoint) -> VkPipelineShaderStageCreateInfo;

        static auto createRasterizationStateInfo(bool depthClamp, bool discardEnabled, VkCullModeFlags cullMode, VkFrontFace frontFace)
            -> VkPipelineRasterizationStateCreateInfo;

        static auto createMultisampleStateInfo(VkSampleCountFlagBits rasterizationSampleCount) -> VkPipelineMultisampleStateCreateInfo;

        static auto createBlendAttachmentState(bool blendEnabled, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor,
            VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp,
            VkColorComponentFlags colorWriteMask) -> VkPipelineColorBlendAttachmentState;

        static auto createColorBlendStateInfo(VkPipelineColorBlendAttachmentState* blendAttachments, bool logicOpEnabled, VkLogicOp logicOp)
            -> VkPipelineColorBlendStateCreateInfo;

        static auto findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
            uint32_t typeBits, VkMemoryPropertyFlags properties) -> int32_t;

        static auto createDevice(VkPhysicalDevice physicalDevice, uint32_t queueIndex) -> VkDevice;
        static auto createPhysicalDevice(VkInstance instance) -> VkPhysicalDevice;

        static auto getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface) -> uint32_t;

        static void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldLayout,
            VkImageLayout newLayout, VkImageSubresourceRange subresourceRange);
        static void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageAspectFlags aspectMask,
            VkImageLayout oldLayout, VkImageLayout newLayout);

        static auto createFrameBuffer(VkDevice logicalDevice, VkImageView colorAttachment, VkImageView depthAttachment,
            VkRenderPass renderPass, uint32_t width, uint32_t height) -> VkFramebuffer;

        static auto createRenderPass(VkDevice logicalDevice, VkFormat colorFormat, VkFormat depthFormat) -> VkRenderPass;

        static auto createCommandPool(VkDevice logicalDevice, uint32_t queueIndex) -> VkCommandPool;

        static auto createCommandBuffer(VkDevice logicalDevice, VkCommandPool commandPool) -> VkCommandBuffer;
        static void submitCommandBuffer(VkQueue queue, VkCommandBuffer buffer);

        static auto getDepthFormat(VkPhysicalDevice device) -> VkFormat;
    };


    // TODO currently supports pools that allocate only one predefined type of sets
    class VulkanDescriptorPool
    {
    public:
        VulkanDescriptorPool() {}
        VulkanDescriptorPool(VkDevice device, VkDescriptorType type, uint32_t descriptorCount, uint32_t maxSetCount);
        VulkanDescriptorPool(const VulkanDescriptorPool& other) = delete;
        VulkanDescriptorPool(VulkanDescriptorPool&& other);

        ~VulkanDescriptorPool();

        auto operator=(const VulkanDescriptorPool& other) -> VulkanDescriptorPool& = delete;
        auto operator=(VulkanDescriptorPool&& other) -> VulkanDescriptorPool&;

        auto allocateSet(VkDescriptorSetLayout layout) const -> VkDescriptorSet;

    private:
        VkDevice device = nullptr;
        VkDescriptorPool pool = nullptr;
    };


    class VulkanDescriptorSetLayoutBuilder
    {
    public:
        explicit VulkanDescriptorSetLayoutBuilder(VkDevice device);

        // TODO other rule-of-five stuff

        void setBinding(uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, VkShaderStageFlagBits stageFlags);

        auto build() -> VkDescriptorSetLayout;

    private:
        VkDevice device = nullptr;
        std::vector<VkDescriptorSetLayoutBinding> bindings;
    };
}