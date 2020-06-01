/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanRenderer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloHash.h"
#include "SoloVulkanFrameBuffer.h"
#include "SoloVulkanSDLDevice.h"
#include "SoloVulkanMaterial.h"
#include "SoloVulkanMesh.h"
#include "SoloCamera.h"
#include "SoloVulkanPipelineContext.h"
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_vulkan.h>

using namespace solo;

static auto contextKey(Transform *transform, Camera *camera, VulkanMaterial *material, VkRenderPass renderPass)
{
    size_t seed = 0;
    const std::hash<void*> hasher;
    combineHash(seed, hasher(transform));
    combineHash(seed, hasher(material));
    combineHash(seed, hasher(camera));
    combineHash(seed, hasher(renderPass));
    return seed;
}

static auto toIndexType(IndexElementSize elementSize) -> VkIndexType
{
	switch (elementSize)
	{
		case IndexElementSize::Bits16: return VK_INDEX_TYPE_UINT16;
		case IndexElementSize::Bits32: return VK_INDEX_TYPE_UINT32;
		default:
			panic(false, "Unknown index element size");
			return VK_INDEX_TYPE_MAX_ENUM;
	}
}

static VulkanSDLDevice *d = nullptr;

VulkanRenderer::VulkanRenderer(Device *device):
    engineDevice_(device)
{
    const auto vkDevice = dynamic_cast<VulkanSDLDevice*>(device);
    const auto instance = vkDevice->instance();
    const auto surface = vkDevice->surface();
    const auto canvasSize = device->canvasSize();

    device_ = VulkanDevice(instance, surface);
    swapchain_ = VulkanSwapchain(device_, static_cast<u32>(canvasSize.x()), static_cast<u32>(canvasSize.y()),
		device->isVsync());

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
	SL_VK_CHECK_RESULT(vkCreateDescriptorPool(device_, &pool_info, nullptr, &descPool));

	VkAttachmentDescription attachment = {};
    attachment.format = device_.colorFormat();
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
	SL_VK_CHECK_RESULT(vkCreateRenderPass(device_, &info, nullptr, &rp));

    d = dynamic_cast<VulkanSDLDevice*>(device);
	
	ImGui_ImplSDL2_InitForVulkan(d->window());
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = d->instance();
    init_info.PhysicalDevice = device_.physical();
    init_info.Device = device_;
    init_info.QueueFamily = device_.queueIndex();
    init_info.Queue = device_.queue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = descPool;
    init_info.Allocator = nullptr;
    init_info.MinImageCount = 2;
    init_info.ImageCount = swapchain_.imageCount();
    init_info.CheckVkResultFn = [](VkResult err) {};

	ImGui_ImplVulkan_Init(&init_info, rp);

	d->onEvent([](SDL_Event &evt) { ImGui_ImplSDL2_ProcessEvent(&evt); });

	auto cmdBuf = VulkanCmdBuffer(device_);
	cmdBuf.begin(true);
	ImGui_ImplVulkan_CreateFontsTexture(cmdBuf);
	cmdBuf.endAndFlush();
	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

VulkanRenderer::~VulkanRenderer()
{
	ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void VulkanRenderer::beginCamera(Camera *camera)
{
	const auto renderTarget = camera->renderTarget().get();
	const auto targetFrameBuffer = dynamic_cast<VulkanFrameBuffer*>(renderTarget);
    const auto currentFrameBuffer = targetFrameBuffer ? targetFrameBuffer->handle() : swapchain_.currentFrameBuffer();
    const auto dimensions = targetFrameBuffer ? targetFrameBuffer->dimensions() : engineDevice_->canvasSize();

	currentCamera_ = camera;
	currentRenderPass_ = targetFrameBuffer ? &targetFrameBuffer->renderPass() : &swapchain_.renderPass();
	
    if (!renderPassContexts_.count(currentRenderPass_))
    {
        renderPassContexts_[currentRenderPass_].cmdBuf = VulkanCmdBuffer(device_);
        renderPassContexts_[currentRenderPass_].completeSemaphore = vk::createSemaphore(device_);
    }

    auto &passContext = renderPassContexts_.at(currentRenderPass_);
    passContext.frameOfLastUse = frame_;
    
    currentCmdBuffer_ = &passContext.cmdBuf;
    currentCmdBuffer_->begin(false);

    currentCmdBuffer_->beginRenderPass(*currentRenderPass_, currentFrameBuffer,
        static_cast<u32>(dimensions.x()), static_cast<u32>(dimensions.y()));

    if (currentCamera_->hasColorClearing())
    {
        const VkClearRect clearRect{{{0, 0}, {static_cast<u32>(dimensions.x()), static_cast<u32>(dimensions.y())}}, 0, 1};
        auto clearColor = currentCamera_->clearColor();
        const VkClearValue clearValue{{clearColor.x(), clearColor.y(), clearColor.z(), clearColor.w()}};
        const auto clearCount = renderTarget
            ? dynamic_cast<VulkanFrameBuffer*>(renderTarget)->colorAttachmentCount()
            : 1;
        for (u32 i = 0; i < clearCount; i++)
            currentCmdBuffer_->clearColorAttachment(i, clearValue, clearRect);
    }

    const auto viewport = currentCamera_->viewport();
    currentCmdBuffer_->setViewport(viewport, 0, 1);
    currentCmdBuffer_->setScissor(viewport);
}

void VulkanRenderer::endCamera(Camera *camera)
{
	ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame(d->window());
    ImGui::NewFrame();

	bool open = true;
	ImGui::ShowDemoWindow(&open);

	ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();

	ImGui_ImplVulkan_RenderDrawData(draw_data, *currentCmdBuffer_);
	
    auto &ctx = renderPassContexts_.at(currentRenderPass_);
    ctx.cmdBuf.endRenderPass();
    ctx.cmdBuf.end();

    vk::queueSubmit(device_.queue(), 1, &prevSemaphore_, 1, &ctx.completeSemaphore, 1, ctx.cmdBuf);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(device_.queue()));

    prevSemaphore_ = ctx.completeSemaphore;

    currentCamera_ = nullptr;
}

void VulkanRenderer::renderMesh(Mesh *mesh, Transform *transform, Material *material)
{
    const auto vkMesh = dynamic_cast<VulkanMesh*>(mesh);
    bindPipelineAndMesh(material, transform, mesh);
    currentCmdBuffer_->draw(vkMesh->minVertexCount(), 1, 0, 0);
}

void VulkanRenderer::renderMeshIndex(Mesh *mesh, u32 index, Transform *transform, Material *material)
{
    const auto vkMesh = dynamic_cast<VulkanMesh*>(mesh);
    bindPipelineAndMesh(material, transform, mesh);
    const auto indexBuffer = vkMesh->indexBuffer(index);
	const auto indexType = toIndexType(mesh->indexBufferElementSize(index));
    currentCmdBuffer_->bindIndexBuffer(indexBuffer, 0, indexType);
    currentCmdBuffer_->drawIndexed(vkMesh->indexBufferElementCount(index), 1, 0, 0, 0);
}

void VulkanRenderer::bindPipelineAndMesh(Material *material, Transform *transform, Mesh *mesh)
{
    const auto vkMaterial = dynamic_cast<VulkanMaterial*>(material);
    const auto vkMesh = dynamic_cast<VulkanMesh*>(mesh);

	const auto key = contextKey(transform, currentCamera_, vkMaterial, *currentRenderPass_);
	if (!pipelineContexts_.count(key))
		pipelineContexts_.emplace(std::make_pair(key, VulkanPipelineContext(&device_, key)));

	auto &context = pipelineContexts_.at(key);
	context.setFrameOfLastUse(frame_);
    
    if (currentPipelineContextKey_ != context.key())
    {
    	context.update(vkMaterial, vkMesh, currentRenderPass_, currentCamera_, transform);
        currentCmdBuffer_->bindPipeline(context.pipeline());
        currentCmdBuffer_->bindDescriptorSet(context.pipeline().layout(), context.descriptorSet());
        currentPipelineContextKey_ = context.key();
    }

    // TODO don't rebind an already bound mesh (for instance when we render mesh indexes)
    for (u32 i = 0; i < vkMesh->vertexBufferCount(); i++)
        currentCmdBuffer_->bindVertexBuffer(i, vkMesh->vertexBuffer(i));
}

void VulkanRenderer::beginFrame()
{
    frame_++;
    currentCamera_ = nullptr;
    currentRenderPass_ = nullptr;
    currentCmdBuffer_ = nullptr;
    currentPipelineContextKey_ = 0;
    prevSemaphore_ = swapchain_.moveNext();
}

void VulkanRenderer::endFrame()
{
	swapchain_.present(device_.queue(), 1, &prevSemaphore_);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(device_.queue()));

    // TODO Naive cleanup, need better
    if (frame_ % 100 == 0)
    {
        cleanupUnusedPipelineContexts();
        cleanupUnusedRenderPassContexts();
    }
}

void VulkanRenderer::cleanupUnusedRenderPassContexts()
{
    bool removed;
    do
    {
        removed = false;
        VulkanRenderPass* key = nullptr;
        for (const auto &p: renderPassContexts_)
        {
	        if (frame_ - p.second.frameOfLastUse >= 100)
            {
                key = p.first;
                removed = true;
            }
        }
        if (removed)
            renderPassContexts_.erase(key);
    }
    while (removed);
}

void VulkanRenderer::cleanupUnusedPipelineContexts()
{
    bool removed;
    do
    {
        removed = false;
        size_t key = 0;
        for (const auto &p: pipelineContexts_)
        {
	        if (frame_ - p.second.frameOfLastUse() >= 100)
            {
                key = p.first;
                removed = true;
            }
        }
        if (removed)
            pipelineContexts_.erase(key);
    }
    while (removed);
}

#endif
