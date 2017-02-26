/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

// NOTE This file, as well as other vulkan files in the engine, is a total hackfest and test playground.
// Consider closing this to prevent your eyes from bleeding.

#include "SoloVulkanRenderer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloSDLVulkanDevice.h"
#include "SoloVulkanCamera.h"
#include "SoloVulkanMaterial.h"
#include "SoloVulkanMesh.h"

using namespace solo;
using namespace vk;


vk::Renderer::Renderer(Device *engineDevice):
    engineDevice(engineDevice)
{
    auto vulkanDevice = dynamic_cast<SDLDevice*>(engineDevice);
    auto instance = vulkanDevice->getInstance();
    auto surface = vulkanDevice->getSurface();
    auto canvasWidth = engineDevice->getSetup().canvasWidth;
    auto canvasHeight = engineDevice->getSetup().canvasHeight;

    physicalDevice.device = getPhysicalDevice(instance);
    vkGetPhysicalDeviceProperties(physicalDevice.device, &physicalDevice.properties);
    vkGetPhysicalDeviceFeatures(physicalDevice.device, &physicalDevice.features);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice.device, &physicalDevice.memProperties);

    auto surfaceFormats = getSurfaceFormats(physicalDevice.device, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);

    auto queueIndex = getQueueIndex(physicalDevice.device, surface);
    this->device = createDevice(physicalDevice.device, queueIndex);

    vkGetDeviceQueue(this->device, queueIndex, 0, &queue);

    depthFormat = getDepthFormat(physicalDevice.device);
    commandPool = createCommandPool(this->device, queueIndex);
    depthStencil = createDepthStencil(this->device, physicalDevice.memProperties, depthFormat, canvasWidth, canvasHeight);
    renderPass = RenderPassBuilder(this->device)
        .withColorAttachment(colorFormat)
        .withDepthAttachment(depthFormat)
        .build();

    swapchain = Swapchain(this->device, physicalDevice.device, surface, renderPass, depthStencil.view,
        canvasWidth, canvasHeight, false, colorFormat, colorSpace);

    semaphores.presentComplete = createSemaphore(this->device);
    semaphores.renderComplete = createSemaphore(this->device);

    renderCmdBuffers.resize(swapchain.getStepCount());
    createCommandBuffers(this->device, commandPool, true, swapchain.getStepCount(), renderCmdBuffers.data());

    createCommandBuffers(this->device, commandPool, false , 1, &mainRenderCmdBuffer);
}


vk::Renderer::~Renderer()
{
    vkFreeCommandBuffers(device, commandPool, 1, &mainRenderCmdBuffer);
    vkFreeCommandBuffers(device, commandPool, renderCmdBuffers.size(), renderCmdBuffers.data());
}


void vk::Renderer::beginFrame()
{
    currentBuffer = swapchain.getNextImageIndex(semaphores.presentComplete);
    prevRenderCommands = std::move(renderCommands);
    renderCommands = std::vector<RenderCommand>(100); // TODO just picked random constant
}


void vk::Renderer::endFrame()
{
    if (dirty)
    {
        recordRenderCmdBuffers();
//        dirty = false; // TODO
    }

    VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &semaphores.presentComplete;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &semaphores.renderComplete;
    submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &renderCmdBuffers[currentBuffer];
    SL_VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

    auto swapchainHandle = swapchain.getHandle();
    VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchainHandle;
	presentInfo.pImageIndices = &currentBuffer;
	presentInfo.pWaitSemaphores = &semaphores.renderComplete;
	presentInfo.waitSemaphoreCount = 1;
    SL_VK_CHECK_RESULT(vkQueuePresentKHR(queue, &presentInfo));

    SL_VK_CHECK_RESULT(vkQueueWaitIdle(queue));
}


// TODO remove after testing
#include "SoloVulkanDescriptorSetLayoutBuilder.h"
#include "SoloVulkanPipeline.h"
#include "SoloVulkanEffect.h"
#include "SoloVulkanBuffer.h"
#include "SoloVulkanDescriptorPool.h"


static bool initialized = false;

void vk::Renderer::applyRenderCommands(VkCommandBuffer buf, VkFramebuffer framebuffer)
{
    // vkCmdExecuteCommands(buf, 1, &mainRenderCmdBuffer);

    const Material *currentMaterial = nullptr;

    for (const auto &cmd: renderCommands)
    {
        switch (cmd.type)
        {
            case RenderCommandType::BeginCamera:
            {
                if (!initialized)
                {
                    // Start secondary command buffer - this should eventually contain all rendering commands
                    VkCommandBufferInheritanceInfo inheritance{};
                    inheritance.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
                    inheritance.renderPass = renderPass;

                    VkCommandBufferBeginInfo beginInfo{};
                    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT | VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
                    beginInfo.pNext = nullptr;
                    beginInfo.pInheritanceInfo = &inheritance;

                    SL_VK_CHECK_RESULT(vkBeginCommandBuffer(mainRenderCmdBuffer, &beginInfo));
                }

                auto color = cmd.camera->getClearColor();
                renderPass.setClear(cmd.camera->isClearColorEnabled(), cmd.camera->isClearDepthEnabled(),
                    {{color.x, color.y, color.z, color.w}},
                    {1, 0});
                    
                auto viewport = cmd.camera->getViewport();
                auto vp = VkViewport{viewport.x, viewport.y, viewport.z, viewport.w, 1, 100};
                
                renderPass.begin(buf, framebuffer, vp.width, vp.height);

                if (!initialized)
                {
                    vkCmdSetViewport(mainRenderCmdBuffer, 0, 1, &vp);

                    VkRect2D scissor;
                    scissor.offset.x = 0;
                    scissor.offset.y = 0;
                    scissor.extent.width = vp.width;
                    scissor.extent.height = vp.height;
                    vkCmdSetScissor(mainRenderCmdBuffer, 0, 1, &scissor);
                }

                break;
            }

            case RenderCommandType::EndCamera:
                if (!initialized)
                    SL_VK_CHECK_RESULT(vkEndCommandBuffer(mainRenderCmdBuffer));

                initialized = true;

                vkCmdExecuteCommands(buf, 1, &mainRenderCmdBuffer);

                renderPass.end(buf);
                break;

            case RenderCommandType::ApplyMaterial:
                currentMaterial = cmd.material;
                break;

            // TODO this is a totally test hackery
            case RenderCommandType::DrawMesh:
            {
                if (!currentMaterial || initialized)
                    continue;

                static bool dirty2 = true;

                if (dirty2)
                {
                    test.descSetLayout = DescriptorSetLayoutBuilder(device)
                        .withBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL_GRAPHICS)
                        .build();

                    auto effect = dynamic_cast<Effect*>(currentMaterial->getEffect());
                    auto vs = effect->getVertexShader();
                    auto fs = effect->getFragmentShader();

                    VkDescriptorSetLayout descSetLayoutHandle = test.descSetLayout;
                    auto builder = PipelineBuilder(device, renderPass, vs, fs)
                        .withDescriptorSetLayouts(&descSetLayoutHandle, 1)
                        .withTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

                    uint32_t vertexSize = 0;
                    for (auto i = 0; i < cmd.mesh->getVertexBufferCount(); ++i)
                    {
                        auto layout = cmd.mesh->getVertexBufferLayout(i);
                        builder.withVertexBinding(i, layout.getSize(), VK_VERTEX_INPUT_RATE_VERTEX);

                        vertexSize += layout.getSize();

                        uint32_t offset = 0;
                        for (auto j = 0; j < layout.getAttributeCount(); ++j)
                        {
                            auto attr = layout.getAttribute(j);
                            auto format = VK_FORMAT_R32_SFLOAT;
                            if (attr.elementCount == 2)
                                format = VK_FORMAT_R32G32_SFLOAT;
                            else if (attr.elementCount == 3)
                                format = VK_FORMAT_R32G32B32_SFLOAT;
                            else if (attr.elementCount == 4)
                                format = VK_FORMAT_R32G32B32A32_SFLOAT;
                            builder.withVertexAttribute(attr.location, i, format, offset);
                            offset += attr.size;
                        }
                    }

                    builder.withVertexSize(vertexSize);

                    test.pipeline = builder.build();
                    test.descriptorPool = DescriptorPool(device, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 1);
                    test.descriptorSet = test.descriptorPool.allocateSet(test.descSetLayout);

                    auto uniformColor = Vector4(0, 0.2f, 0.8f, 1);
                    test.uniformBuffer = Buffer(device, sizeof(Vector4), Buffer::Uniform | Buffer::Host, physicalDevice.memProperties);
                    test.uniformBuffer.update(&uniformColor);

                    VkDescriptorBufferInfo uboInfo{};
                    uboInfo.buffer = test.uniformBuffer.getHandle();
                    uboInfo.offset = 0;
                    uboInfo.range = sizeof(Vector3);

                    VkWriteDescriptorSet descriptorWrite{};
                    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrite.dstSet = test.descriptorSet;
                    descriptorWrite.dstBinding = 0;
                    descriptorWrite.dstArrayElement = 0;
                    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    descriptorWrite.descriptorCount = 1;
                    descriptorWrite.pBufferInfo = &uboInfo;
                    descriptorWrite.pImageInfo = nullptr; // Optional
                    descriptorWrite.pTexelBufferView = nullptr; // Optional

                    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);

                    test.vertexCount = cmd.mesh->getMinVertexCount();

                    dirty2 = false;
                }

                vkCmdBindPipeline(mainRenderCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, test.pipeline);
                vkCmdBindDescriptorSets(mainRenderCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, test.pipeline.getLayout(), 0, 1, &test.descriptorSet, 0, nullptr);
                
                std::vector<VkBuffer> vertexBuffers = {cmd.mesh->getVertexBuffer(0), cmd.mesh->getVertexBuffer(1)};
                std::vector<VkDeviceSize> offsets = {0, 0};
	            vkCmdBindVertexBuffers(mainRenderCmdBuffer, 0, 2, vertexBuffers.data(), offsets.data());

                vkCmdBindIndexBuffer(mainRenderCmdBuffer, cmd.mesh->getPartBuffer(0), 0, VK_INDEX_TYPE_UINT16);
                vkCmdDrawIndexed(mainRenderCmdBuffer, cmd.mesh->getPartIndexElementCount(0), 1, 0, 0, 1);
                
                break;
            }

            default:
                break;
        }
    }
}


void vk::Renderer::recordRenderCmdBuffers()
{
    initialized = false;

    for (size_t i = 0; i < renderCmdBuffers.size(); i++)
    {
        auto buf = renderCmdBuffers[i];

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        SL_VK_CHECK_RESULT(vkBeginCommandBuffer(buf, &beginInfo));

        applyRenderCommands(buf, swapchain.getFramebuffer(i));

        SL_VK_CHECK_RESULT(vkEndCommandBuffer(buf));
    }
}


#endif
