/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
#   define SL_VK_CHECK_RESULT(vkCall, ...) SL_PANIC_IF(vkCall != VK_SUCCESS, __VA_ARGS__)
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
                swap(other);
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
                swap(other);
                return *this;
            }

            void operator=(T other)
            {
                if (other != handle)
                {
                    cleanup();
                    handle = other;
                }
            }

            auto operator&() const -> const T*
            {
                return &handle;
            }

            auto cleanAndExpose() -> T*
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

            void swap(Resource<T> &other) noexcept
            {
                std::swap(handle, other.handle);
                std::swap(del, other.del);
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
        void createCommandBuffers(VkDevice device, VkCommandPool commandPool, bool primary, uint32_t count, VkCommandBuffer *result);
    
        auto findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, uint32_t typeBits,
            VkMemoryPropertyFlags properties) -> int32_t;
    
        auto createRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat) -> Resource<VkRenderPass>;
    
        auto createFrameBuffer(VkDevice device, VkImageView colorAttachment, VkImageView depthAttachment,
            VkRenderPass renderPass, uint32_t width, uint32_t height) -> Resource<VkFramebuffer>;
    
        auto createShader(VkDevice device, const void *data, uint32_t size) -> Resource<VkShaderModule>;
        auto createShaderStageInfo(bool vertex, VkShaderModule shader, const char *entryPoint) -> VkPipelineShaderStageCreateInfo;
    
        auto createDepthStencil(VkDevice device, VkPhysicalDeviceMemoryProperties physicalDeviceMemProps,
            VkFormat depthFormat, uint32_t canvasWidth, uint32_t canvasHeight) -> DepthStencil;
    }
}

#endif
