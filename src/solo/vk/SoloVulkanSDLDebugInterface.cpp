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

	VkDescriptorPoolSize pool_sizes[] =
    {
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
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
    pool_info.poolSizeCount = static_cast<uint32_t>(IM_ARRAYSIZE(pool_sizes));
    pool_info.pPoolSizes = pool_sizes;

	VkDescriptorPool descPool;
	SL_VK_CHECK_RESULT(vkCreateDescriptorPool(renderer_->device(), &pool_info, nullptr, &descPool));

	VkAttachmentDescription attachment = {};
    attachment.format = renderer_->device().colorFormat();
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    VkAttachmentReference color_attachment = {};
    color_attachment.attachment = 0;
    color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment;
    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    VkRenderPassCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 1;
    info.pAttachments = &attachment;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 1;
    info.pDependencies = &dependency;

	VkRenderPass rp;
	SL_VK_CHECK_RESULT(vkCreateRenderPass(renderer_->device(), &info, nullptr, &rp));

	ImGui_ImplSDL2_InitForVulkan(device_->window());
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = device_->instance();
    init_info.PhysicalDevice = renderer_->device().physical();
    init_info.Device = renderer_->device();
    init_info.QueueFamily = renderer_->device().queueIndex();
    init_info.Queue = renderer_->device().queue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = descPool;
    init_info.Allocator = nullptr;
    init_info.MinImageCount = 2;
    init_info.ImageCount = renderer_->swapchain().imageCount();
    init_info.CheckVkResultFn = [](VkResult) {};

	ImGui_ImplVulkan_Init(&init_info, rp);

	device_->onEvent([](SDL_Event &evt) { ImGui_ImplSDL2_ProcessEvent(&evt); });

	auto cmdBuf = VulkanCmdBuffer(renderer_->device());
	cmdBuf.begin(true);
	ImGui_ImplVulkan_CreateFontsTexture(cmdBuf);
	cmdBuf.endAndFlush();
	ImGui_ImplVulkan_DestroyFontUploadObjects();
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

#endif
