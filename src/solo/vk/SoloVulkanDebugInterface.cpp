/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanDebugInterface.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"
#include "SoloVulkanRenderer.h"
#include "SoloVulkanDevice.h"
#include "SoloVulkanCmdBuffer.h"
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_vulkan.h>

using namespace solo;

VulkanDebugInterface::VulkanDebugInterface(Device *device):
	SDLDebugInterface(device),
	device_(dynamic_cast<VulkanDevice*>(device)),
	renderer_(dynamic_cast<VulkanRenderer*>(device->renderer()))
{
	ImGui_ImplSDL2_InitForVulkan(device_->window());

	// Desc pool
	{
		std::vector<VkDescriptorPoolSize> poolSizes = {
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
	        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
	        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
	        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
	        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
	        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
	        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
	        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
	        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
	        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
	        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};
		
	    VkDescriptorPoolCreateInfo poolInfo = {};
	    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	    poolInfo.maxSets = 1000 * poolSizes.size();
	    poolInfo.poolSizeCount = static_cast<u32>(poolSizes.size());
	    poolInfo.pPoolSizes = poolSizes.data();

		descPool_ = VulkanResource<VkDescriptorPool>{renderer_->device(), vkDestroyDescriptorPool};
		SL_VK_CHECK_RESULT(vkCreateDescriptorPool(renderer_->device(), &poolInfo, nullptr, descPool_.cleanRef()));
	}

	// Render pass
	{
		const auto cfg = VulkanRenderPassConfig()
			.addColorAttachment(renderer_->device().colorFormat(), VK_IMAGE_LAYOUT_GENERAL); // TODO proper layout
		renderPass_ = VulkanRenderPass(renderer_->device(), cfg);
	}

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = device_->instance();
    initInfo.PhysicalDevice = renderer_->device().physical();
    initInfo.Device = renderer_->device();
    initInfo.QueueFamily = renderer_->device().queueIndex();
    initInfo.Queue = renderer_->device().queue();
    initInfo.PipelineCache = VK_NULL_HANDLE;
    initInfo.DescriptorPool = descPool_;
    initInfo.Allocator = nullptr;
    initInfo.MinImageCount = 2;
    initInfo.ImageCount = renderer_->swapchain().imageCount();
    initInfo.CheckVkResultFn = [](VkResult) {};

	ImGui_ImplVulkan_Init(&initInfo, renderPass_.handle());

	// Load fonts
	auto cmdBuf = VulkanCmdBuffer(renderer_->device());
	cmdBuf.begin(true);
	ImGui_ImplVulkan_CreateFontsTexture(cmdBuf);
	cmdBuf.endAndFlush();
	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

VulkanDebugInterface::~VulkanDebugInterface()
{
	ImGui_ImplVulkan_Shutdown();
}

void VulkanDebugInterface::renderInto(VkCommandBuffer targetCmdBuffer) const
{
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), targetCmdBuffer);
}

#endif
