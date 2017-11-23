/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#ifdef SL_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan.h>
#include <functional>
#include <cassert>

namespace solo
{
    template <class T>
    class VulkanResource
    {
    public:
        VulkanResource() {}
        VulkanResource(const VulkanResource<T> &other) = delete;
        VulkanResource(VulkanResource<T> &&other) noexcept
        {
            swap(other);
        }

        explicit VulkanResource(std::function<void(T, VkAllocationCallbacks*)> del)
        {
            this->del = [=](T handle) { del(handle, nullptr); };
        }

        VulkanResource(VkInstance instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> del)
        {
            this->del = [instance, del](T obj) { del(instance, obj, nullptr); };
        }

        VulkanResource(VkDevice device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> del)
        {
            this->del = [device, del](T obj) { del(device, obj, nullptr); };
        }

        VulkanResource(VkDevice device, VkCommandPool cmdPool, std::function<void(VkDevice, VkCommandPool, u32, T*)> del)
        {
            this->del = [device, cmdPool, del](T obj) { del(device, cmdPool, 1, &obj); };
        }

        ~VulkanResource()
        {
            cleanup();
        }

        auto operator=(VulkanResource<T> other) noexcept -> VulkanResource<T>&
        {
            swap(other);
            return *this;
        }

        auto operator&() const -> const T*
        {
            return &handle;
        }

        auto operator&() -> T*
        {
            return &handle;
        }

        auto cleanRef() -> T*
        {
            ensureInitialized();
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
            {
                ensureInitialized();
                del(handle);
            }
            handle = VK_NULL_HANDLE;
        }

        void ensureInitialized()
        {
            assert(del /* Calling cleanup() on a Resource with empty deleter */);
        }

        void swap(VulkanResource<T> &other) noexcept
        {
            std::swap(handle, other.handle);
            std::swap(del, other.del);
        }
    };
}

#endif