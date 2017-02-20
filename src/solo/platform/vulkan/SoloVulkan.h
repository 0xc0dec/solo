/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include <vector>
#include <functional>

#ifdef SL_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan.h>

#ifdef SL_ERR_CHECK
#   define SL_VK_CHECK_RESULT(vkCall, ...) SL_ERR_CHECK_BLOCK(if (vkCall != VK_SUCCESS) SL_ERR(__VA_ARGS__))
#else
#   define SL_VK_CHECK_RESULT(vkCall) vkCall
#endif

namespace solo
{
    namespace vk
    {
        template <class T>
        class Resource
        {
        public:
            Resource(): Resource([](T, VkAllocationCallbacks*) {})
            {
            }

            Resource(const Resource<T> &other) = delete;
            Resource(Resource<T> &&other) noexcept
            {
                cleanup();
                handle = std::move(other.handle);
                del = std::move(other.del);
                other.handle = VK_NULL_HANDLE;
            }

            explicit Resource(std::function<void(T, VkAllocationCallbacks*)> del)
            {
                this->del = [=](T handle) { del(handle, nullptr); };
            }

            Resource(VkInstance instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> del)
            {
                this->del = [instance, del](T obj) { del(instance, obj, nullptr); };
            }

            Resource(VkDevice device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> del)
            {
                this->del = [device, del](T obj) { del(device, obj, nullptr); };
            }

            ~Resource()
            {
                cleanup();
            }

            auto operator=(Resource<T> other) noexcept -> Resource<T>&
            {
                std::swap(handle, other.handle);
                std::swap(del, other.del);
                return *this;
            }

            auto operator&() const -> const T*
            {
                return &handle;
            }

            auto replace() -> T*
            {
                cleanup();
                return &handle;
            }

            operator T() const
            {
                return handle;
            }

            operator bool() const
            {
                return handle != VK_NULL_HANDLE;
            }

            void operator=(T other)
            {
                if (other != handle)
                {
                    cleanup();
                    handle = other;
                }
            }

            template<typename V>
            bool operator==(V rhs)
            {
                return handle == T(rhs);
            }

        private:
            T handle = VK_NULL_HANDLE;
            std::function<void(T)> del;

            void cleanup()
            {
                if (handle != VK_NULL_HANDLE)
                    del(handle);
                handle = VK_NULL_HANDLE;
            }
        };

        struct DepthStencil
        {
            Resource<VkImage> image;
            Resource<VkDeviceMemory> mem;
            Resource<VkImageView> view;
        };

        auto getPhysicalDevice(VkInstance instance) -> VkPhysicalDevice;
    
        auto createDevice(VkPhysicalDevice physicalDevice, uint32_t queueIndex) -> Resource<VkDevice>;
    
        auto getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface) -> std::tuple<VkFormat, VkColorSpaceKHR>;
    
        auto createSemaphore(VkDevice device) -> Resource<VkSemaphore>;
    
        auto getPhysicalDevice(VkInstance instance) -> VkPhysicalDevice;
    
        auto getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface) -> uint32_t;
    
        auto getDepthFormat(VkPhysicalDevice device) -> VkFormat;
    
        auto createCommandPool(VkDevice device, uint32_t queueIndex) -> Resource<VkCommandPool>;
    
        void submitCommandBuffer(VkQueue queue, VkCommandBuffer buffer);
        void createCommandBuffers(VkDevice logicalDevice, VkCommandPool commandPool, uint32_t count, VkCommandBuffer *result);
        void destroyCommandBuffers(VkDevice device, VkCommandPool commandPool, VkCommandBuffer* buffers, uint32_t count);
    
        auto findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, uint32_t typeBits,
            VkMemoryPropertyFlags properties) -> int32_t;
    
        auto createRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat) -> Resource<VkRenderPass>;
    
        auto createFrameBuffer(VkDevice device, VkImageView colorAttachment, VkImageView depthAttachment,
            VkRenderPass renderPass, uint32_t width, uint32_t height) -> Resource<VkFramebuffer>;
    
        auto createShader(VkDevice device, const std::vector<uint8_t> &data) -> Resource<VkShaderModule>;
        auto createShaderStageInfo(bool vertex, VkShaderModule shader, const char *entryPoint) -> VkPipelineShaderStageCreateInfo;
    
        auto createDepthStencil(VkDevice device, VkPhysicalDeviceMemoryProperties physicalDeviceMemProps,
            VkFormat depthFormat, uint32_t canvasWidth, uint32_t canvasHeight) -> DepthStencil;
    
        auto createRasterizationStateInfo(bool depthClamp, bool discardEnabled, VkCullModeFlags cullMode, VkFrontFace frontFace)
            -> VkPipelineRasterizationStateCreateInfo;
    
        auto createMultisampleStateInfo(VkSampleCountFlagBits rasterizationSampleCount) -> VkPipelineMultisampleStateCreateInfo;
    
        auto createBlendAttachmentState(bool blendEnabled, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor,
            VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp,
            VkColorComponentFlags colorWriteMask) -> VkPipelineColorBlendAttachmentState;
    
        auto createColorBlendStateInfo(VkPipelineColorBlendAttachmentState* blendAttachments, bool logicOpEnabled, VkLogicOp logicOp)
            -> VkPipelineColorBlendStateCreateInfo;
    }
}

#endif
