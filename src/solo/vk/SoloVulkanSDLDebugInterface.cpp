/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanSDLDebugInterface.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"
#include "SoloVulkanRenderer.h"
#include "SoloVulkanSDLDevice.h"
#include "SoloVulkanCmdBuffer.h"
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_vulkan.h>

using namespace solo;

VulkanSDLDebugInterface::VulkanSDLDebugInterface(Device *device):
	device_(dynamic_cast<VulkanSDLDevice*>(device)),
	renderer_(dynamic_cast<VulkanRenderer*>(device->renderer()))
{
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

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
	    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	    poolInfo.pPoolSizes = poolSizes.data();

		SL_VK_CHECK_RESULT(vkCreateDescriptorPool(renderer_->device(), &poolInfo, nullptr, descPool_.cleanRef()));
	}

	// Render pass
	{
		const auto cfg = VulkanRenderPassConfig()
			.addColorAttachment(renderer_->device().colorFormat(), VK_IMAGE_LAYOUT_GENERAL); // TODO proper layout
		renderPass_ = VulkanRenderPass(renderer_->device(), cfg);
	}

	ImGui_ImplSDL2_InitForVulkan(device_->window());
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = device_->instance();
    init_info.PhysicalDevice = renderer_->device().physical();
    init_info.Device = renderer_->device();
    init_info.QueueFamily = renderer_->device().queueIndex();
    init_info.Queue = renderer_->device().queue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = descPool_;
    init_info.Allocator = nullptr;
    init_info.MinImageCount = 2;
    init_info.ImageCount = renderer_->swapchain().imageCount();
    init_info.CheckVkResultFn = [](VkResult) {};

	ImGui_ImplVulkan_Init(&init_info, renderPass_.handle());

	device_->onEvent([](SDL_Event &evt) { ImGui_ImplSDL2_ProcessEvent(&evt); });

	// Load fonts
	{
		auto cmdBuf = VulkanCmdBuffer(renderer_->device());
		cmdBuf.begin(true);
		ImGui_ImplVulkan_CreateFontsTexture(cmdBuf);
		cmdBuf.endAndFlush();
		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}

	finishSemaphore_ = vk::createSemaphore(renderer_->device());
	renderCmdBuf_ = VulkanCmdBuffer(renderer_->device());
}

VulkanSDLDebugInterface::~VulkanSDLDebugInterface()
{
	ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void VulkanSDLDebugInterface::beginFrame()
{
}

void VulkanSDLDebugInterface::endFrame()
{
	
}

auto VulkanSDLDebugInterface::render(VkSemaphore waitSemaphore) -> VkSemaphore
{
	renderCmdBuf_.begin(false);
	
	renderCmdBuf_.beginRenderPass(renderPass_, renderer_->swapchain().currentFrameBuffer(),
		device_->canvasSize().x(), device_->canvasSize().y());
	const auto viewport = Vector4(0, 0, device_->canvasSize().x(), device_->canvasSize().y());
	renderCmdBuf_.setViewport(viewport, 0, 1);
	renderCmdBuf_.setScissor(viewport);
	
    ImGui_ImplSDL2_NewFrame(device_->window());
    ImGui::NewFrame();

	// TODO remove
	bool open = true;
	ImGui::ShowDemoWindow(&open);
	
	ImGui::Render();
	const auto data = ImGui::GetDrawData();
	ImGui_ImplVulkan_RenderDrawData(data, renderCmdBuf_);

	renderCmdBuf_.endRenderPass();
	renderCmdBuf_.end();

	vk::queueSubmit(renderer_->device().queue(), 1, &waitSemaphore, 1, &finishSemaphore_, 1, renderCmdBuf_);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(renderer_->device().queue()));

	return finishSemaphore_;
}

#endif
