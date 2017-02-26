/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

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
    createCommandBuffers(this->device, commandPool, swapchain.getStepCount(), renderCmdBuffers.data());
}


vk::Renderer::~Renderer()
{
    vkFreeCommandBuffers(device, commandPool, renderCmdBuffers.size(), renderCmdBuffers.data());
}


void vk::Renderer::beginFrame()
{
//    currentBuffer = swapchain.getNextImageIndex(semaphores.presentComplete);
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


void vk::Renderer::applyRenderCommands(VkCommandBuffer buf, VkFramebuffer framebuffer)
{
    const Material *currentMaterial = nullptr;

    for (const auto &cmd: renderCommands)
    {
        switch (cmd.type)
        {
            case RenderCommandType::BeginCamera:
            {
                auto color = cmd.camera->getClearColor();
                renderPass.setClear(cmd.camera->isClearColorEnabled(), cmd.camera->isClearDepthEnabled(),
                    {{color.x, color.y, color.z, color.w}},
                    {1, 0});
                    
                auto viewport = cmd.camera->getViewport();
                auto vp = VkViewport{viewport.x, viewport.y, viewport.z, viewport.w, 1, 100};
                
                renderPass.begin(buf, framebuffer, vp.width, vp.height);
                vkCmdSetViewport(buf, 0, 1, &vp);

                // TODO this is temp
                VkRect2D scissor;
                scissor.offset.x = 0;
                scissor.offset.y = 0;
                scissor.extent.width = vp.width;
                scissor.extent.height = vp.height;
                vkCmdSetScissor(buf, 0, 1, &scissor);

                break;
            }

            case RenderCommandType::EndCamera:
                renderPass.end(buf);
                break;

            case RenderCommandType::ApplyMaterial:
                currentMaterial = cmd.material;
                break;

            // TODO this is a totally test hackery
            case RenderCommandType::DrawMesh:
            {
                if (!currentMaterial)
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

                    auto layout = cmd.mesh->getVertexBufferLayout(0);
                    uint32_t offset = 0;
                    for (auto i = 0; i < layout.getAttributeCount(); ++i)
                    {
                        auto attr = layout.getAttribute(i);
                        auto format = VK_FORMAT_R32_SFLOAT;
                        if (attr.elementCount == 2)
                            format = VK_FORMAT_R32G32_SFLOAT;
                        if (attr.elementCount == 3)
                            format = VK_FORMAT_R32G32B32_SFLOAT;
                        if (attr.elementCount == 4)
                            format = VK_FORMAT_R32G32B32A32_SFLOAT;
                        builder.withVertexAttribute(attr.location, 0, format, offset);
                        offset += attr.size;
                    }

                    builder.withVertexSize(layout.getSize());

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

                    test.vertexCount = 3;// static_cast<uint32_t>(tri1.size()); TODO

                    dirty2 = false;
                }

                vkCmdBindPipeline(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, test.pipeline);

                vkCmdBindDescriptorSets(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, test.pipeline.getLayout(), 0, 1, &test.descriptorSet, 0, nullptr);

                VkDeviceSize offset = 0;
                
                vkCmdBindIndexBuffer(buf, cmd.mesh->getPartBuffer(0), 0, VK_INDEX_TYPE_UINT16);

                auto handle = cmd.mesh->getVertexBuffer(0);
	            vkCmdBindVertexBuffers(buf, 0, 1, &handle, &offset);
                vkCmdDrawIndexed(buf, 3, 1, 0, 0, 1);

                handle = cmd.mesh->getVertexBuffer(1);
	            vkCmdBindVertexBuffers(buf, 0, 1, &handle, &offset);
                vkCmdDraw(buf, test.vertexCount, 1, 0, 0);
                
                break;
            }

            default:
                break;
        }
    }
}


void vk::Renderer::recordRenderCmdBuffers()
{
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
