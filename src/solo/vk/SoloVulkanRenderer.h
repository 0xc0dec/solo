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
#include "SoloVulkanRenderPass.h"
#include "SoloRenderCommand.h"
#include "SoloVulkan.h"
#include "SoloVulkanPipeline.h"
#include "SoloVulkanBuffer.h"
#include "SoloVulkanDescriptorPool.h"

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
            auto getDevice() const -> VkDevice;
            auto getPhysicalDeviceMemProps() const -> VkPhysicalDeviceMemoryProperties;
            auto getQueue() const -> VkQueue;
            auto getCommandPool() const -> VkCommandPool;

            void addRenderCommand(const RenderCommand &cmd) override final;

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

            std::vector<VkCommandBuffer> swapchainCmdBuffers;
            VkCommandBuffer mainRenderCmdBuffer;
            uint32_t currentSwapchainStep = 0;

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
            } test;

            void applyRenderCommands(VkCommandBuffer buf);
            void recordCmdBuffers();
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
