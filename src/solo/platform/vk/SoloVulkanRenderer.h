/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVulkanDescriptorPool.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloRenderer.h"
#include "SoloVulkanSwapchain.h"
#include "SoloVulkanRenderPass.h"
#include "SoloVulkan.h"
#include "SoloVulkanRenderCommand.h"
#include "SoloVulkanPipeline.h"
#include "SoloVulkanBuffer.h"

namespace solo
{
    class Device;

    namespace vk
    {
        class Camera;
        class Mesh;

        class Renderer final : public solo::Renderer
        {
        public:
            explicit Renderer(Device *device);
            ~Renderer();

            // TODO avoid these?
            auto getDevice() const -> VkDevice;
            auto getPhysicalDeviceMemProps() const -> VkPhysicalDeviceMemoryProperties;
            auto getQueue() const -> VkQueue;
            auto getCommandPool() const -> VkCommandPool;

            void addRenderCommand(const RenderCommand &cmd);

        protected:
            void beginFrame() override final;
            void endFrame() override final;

        private:
            VkFormat depthFormat = VK_FORMAT_UNDEFINED;
            VkFormat colorFormat = VK_FORMAT_UNDEFINED;
            VkColorSpaceKHR colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

            struct
            {
                VkPhysicalDevice device = nullptr;
                VkPhysicalDeviceFeatures features;
                VkPhysicalDeviceProperties properties;
                VkPhysicalDeviceMemoryProperties memProperties;
            } physicalDevice;

            Device *engineDevice = nullptr;
            Resource<VkDevice> device;
            VkQueue queue = nullptr;
            Resource<VkCommandPool> commandPool;
            DepthStencil depthStencil;
            RenderPass renderPass;
            Swapchain swapchain;

            struct
            {
                Resource<VkSemaphore> renderComplete;
                Resource<VkSemaphore> presentComplete;
            } semaphores;

            std::vector<VkCommandBuffer> renderCmdBuffers;
            uint32_t currentBuffer = 0;

            bool dirty = true;

            // TODO currently we just always recreate render plan.
            // In the future these command sets should be compared to see it something
            // really needs to be done
            std::vector<RenderCommand> renderCommands;
            std::vector<RenderCommand> prevRenderCommands;

            struct
            {
                Resource<VkDescriptorSetLayout> descSetLayout;
                DescriptorPool descriptorPool;
                Buffer uniformBuffer;
                Pipeline pipeline;
                VkDescriptorSet descriptorSet;
                uint32_t vertexCount;
                Buffer indexBuffer;
            } test;

            void applyRenderCommands(VkCommandBuffer buf, VkFramebuffer framebuffer);
            void recordRenderCmdBuffers();
        };

        inline auto Renderer::getDevice() const -> VkDevice
        {
            return device;
        }

        inline auto Renderer::getPhysicalDeviceMemProps() const -> VkPhysicalDeviceMemoryProperties
        {
            return physicalDevice.memProperties;
        }

        inline auto Renderer::getQueue() const -> VkQueue
        {
            return queue;
        }

        inline auto Renderer::getCommandPool() const -> VkCommandPool
        {
            return commandPool;
        }

        inline void Renderer::addRenderCommand(const RenderCommand &cmd)
        {
            renderCommands.push_back(cmd);
        }
    }
}

#endif
