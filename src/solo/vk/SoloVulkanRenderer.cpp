/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanRenderer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloHash.h"
#include "SoloVulkanFrameBuffer.h"
#include "SoloVulkanSDLDevice.h"
#include "SoloVulkanMaterial.h"
#include "SoloVulkanMesh.h"
#include "SoloVulkanEffect.h"
#include "SoloVulkanDescriptorSetLayoutBuilder.h"
#include "SoloVulkanDescriptorSetUpdater.h"
#include "SoloVulkanTexture.h"
#include "SoloCamera.h"

using namespace solo;

static auto getVertexAttributeFormat(const VertexAttribute &attr) -> VkFormat
{
    switch (attr.elementCount)
    {
        case 1: return VK_FORMAT_R32_SFLOAT;
        case 2: return VK_FORMAT_R32G32_SFLOAT;
        case 3: return VK_FORMAT_R32G32B32_SFLOAT;
        case 4: return VK_FORMAT_R32G32B32A32_SFLOAT;
    }

    SL_DEBUG_PANIC(true, "Unsupported vertex attribute element count");
    return VK_FORMAT_UNDEFINED;
}

static auto getPipelineContextKey(Transform *transform, Camera *camera, VulkanMaterial *material, VkRenderPass renderPass)
{
    size_t seed = 0;
    const std::hash<void*> hasher;
    combineHash(seed, hasher(transform));
    combineHash(seed, hasher(material));
    combineHash(seed, hasher(camera));
    combineHash(seed, hasher(renderPass));
    return seed;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackFunc(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
    u64 obj, size_t location, s32 code, const s8 *layerPrefix, const s8 *msg, void *userData)
{
    SL_DEBUG_LOG("Vulkan: ", msg);
    return VK_FALSE;
}

static auto createDebugCallback(VkInstance instance, PFN_vkDebugReportCallbackEXT callbackFunc) -> VulkanResource<VkDebugReportCallbackEXT>
{
    VkDebugReportCallbackCreateInfoEXT createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = callbackFunc;

    const auto create = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
    SL_DEBUG_PANIC(!create, "Unable to load pointer to vkCreateDebugReportCallbackEXT");

    const auto destroy = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
    SL_DEBUG_PANIC(!destroy, "Unable to load pointer to vkDestroyDebugReportCallbackEXT");

    VulkanResource<VkDebugReportCallbackEXT> result{instance, destroy};
    SL_VK_CHECK_RESULT(create(instance, &createInfo, nullptr, result.cleanRef()));

    return result;
}

static auto createDevice(VkPhysicalDevice physicalDevice, u32 queueIndex) -> VulkanResource<VkDevice>
{
    vec<float> queuePriorities = {0.0f};
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = queuePriorities.data();

    vec<const s8*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkPhysicalDeviceFeatures enabledFeatures{};
    enabledFeatures.samplerAnisotropy = true;

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
    deviceCreateInfo.enabledExtensionCount = static_cast<u32>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    VulkanResource<VkDevice> result{vkDestroyDevice};
    SL_VK_CHECK_RESULT(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, result.cleanRef()));

    return result;
}

static auto getPhysicalDevice(VkInstance instance) -> VkPhysicalDevice
{
    u32 gpuCount = 0;
    SL_VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));

    vec<VkPhysicalDevice> devices(gpuCount);
    SL_VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, devices.data()));

    return devices[0]; // Taking first one for simplicity
}

static auto getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface) -> std::tuple<VkFormat, VkColorSpaceKHR>
{
    u32 count;
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr));

    vec<VkSurfaceFormatKHR> formats(count);
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data()));

    if (count == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
        return {VK_FORMAT_B8G8R8A8_UNORM, formats[0].colorSpace};
    return {formats[0].format, formats[0].colorSpace};
}

static auto getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface) -> u32
{
    u32 count;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

    vec<VkQueueFamilyProperties> queueProps;
    queueProps.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueProps.data());

    vec<VkBool32> presentSupported(count);
    for (u32 i = 0; i < count; i++)
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupported[i]));

    // TODO support for separate rendering and presenting queues
    for (u32 i = 0; i < count; i++)
    {
        if (queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupported[i] == VK_TRUE)
            return i;
    }

    SL_DEBUG_PANIC(true, "Unable to find queue index");
    return 0;
}

static auto createCommandPool(VkDevice device, u32 queueIndex) -> VulkanResource<VkCommandPool>
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VulkanResource<VkCommandPool> commandPool{device, vkDestroyCommandPool};
    SL_VK_CHECK_RESULT(vkCreateCommandPool(device, &poolInfo, nullptr, commandPool.cleanRef()));

    return commandPool;
}

static auto getDepthFormat(VkPhysicalDevice device) -> VkFormat
{
    vec<VkFormat> depthFormats =
    {
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM
    };

    for (auto &format : depthFormats)
    {
        if (vk::isFormatSupported(device, format, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT))
            return format;
    }

    SL_DEBUG_PANIC(true, "Unable to pick depth format");
    return VK_FORMAT_UNDEFINED;
}

VulkanRenderer::VulkanRenderer(Device *engineDevice):
    engineDevice_(engineDevice)
{
    const auto vulkanDevice = dynamic_cast<VulkanSDLDevice*>(engineDevice);
    const auto instance = vulkanDevice->instance();
    const auto surface = vulkanDevice->surface();
    const auto canvasSize = engineDevice->canvasSize();

#ifdef SL_DEBUG
    debugCallback_ = createDebugCallback(instance, debugCallbackFunc);
#endif
    physicalDevice_ = ::getPhysicalDevice(instance);
    vkGetPhysicalDeviceProperties(physicalDevice_, &physicalProperties_);
    vkGetPhysicalDeviceFeatures(physicalDevice_, &physicalFeatures_);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice_, &physicalMemoryFeatures_);

    auto surfaceFormats = getSurfaceFormats(physicalDevice_, surface);
    colorFormat_ = std::get<0>(surfaceFormats);
    colorSpace_ = std::get<1>(surfaceFormats);
    depthFormat_ = ::getDepthFormat(physicalDevice_);

    const auto queueIndex = getQueueIndex(physicalDevice_, surface);
    device_ = createDevice(physicalDevice_, queueIndex);
    vkGetDeviceQueue(device_, queueIndex, 0, &queue_);

    commandPool_ = createCommandPool(device_, queueIndex);
    swapchain_ = VulkanSwapchain(this, vulkanDevice, static_cast<u32>(canvasSize.x()), static_cast<u32>(canvasSize.y()), engineDevice->isVsync());
}

void VulkanRenderer::beginCamera(Camera *camera, FrameBuffer *renderTarget)
{
    currentCamera_ = camera;
    currentRenderPass_ = &swapchain_.renderPass();
    
    auto currentFrameBuffer = swapchain_.currentFrameBuffer();
    auto dimensions = engineDevice_->canvasSize();
    vec<VkClearAttachment> clearAttachments = {{VK_IMAGE_ASPECT_COLOR_BIT, 0, {{0, 0, 0, 1}}}}; // TODO avoid mem allocation

    if (renderTarget)
    {
        const auto targetFrameBuffer = static_cast<VulkanFrameBuffer*>(renderTarget);
        currentRenderPass_ = &targetFrameBuffer->renderPass();
        currentFrameBuffer = targetFrameBuffer->handle();
        dimensions = targetFrameBuffer->getDimensions();
        clearAttachments.resize(targetFrameBuffer->colorAttachmentCount());
    }

    if (!renderPassContexts_.count(currentRenderPass_))
    {
        renderPassContexts_[currentRenderPass_].cmdBuffer = vk::createCommandBuffer(device_, commandPool_);
        renderPassContexts_[currentRenderPass_].completeSemaphore = vk::createSemaphore(device_);
    }

    auto &passContext = renderPassContexts_.at(currentRenderPass_);
    passContext.frameOfLastUse = frame_;
    
    currentCmdBuffer_ = passContext.cmdBuffer;
    vk::beginCommandBuffer(currentCmdBuffer_, false);

    currentRenderPass_->begin(currentCmdBuffer_, currentFrameBuffer, static_cast<u32>(dimensions.x()), static_cast<u32>(dimensions.y()));

    if (currentCamera_->hasColorClearing())
    {
        VkClearRect clearRect{};
        clearRect.layerCount = 1;
        clearRect.rect.offset = {0, 0};
        clearRect.rect.extent = {static_cast<u32>(dimensions.x()), static_cast<u32>(dimensions.y())};
        auto clearColor = currentCamera_->getClearColor();
        for (u32 i = 0; i < clearAttachments.size(); ++i)
        {
            clearAttachments[i].aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            clearAttachments[i].colorAttachment = i;
            clearAttachments[i].clearValue = {{clearColor.x(), clearColor.y(), clearColor.z(), clearColor.w()}};
        }
        if (!clearAttachments.empty())
            vkCmdClearAttachments(currentCmdBuffer_, static_cast<u32>(clearAttachments.size()), clearAttachments.data(), 1, &clearRect);
    }

    const auto viewport = currentCamera_->getViewport();
    VkViewport vp{viewport.x(), viewport.y(), viewport.z(), viewport.w(), 0, 1};
    vkCmdSetViewport(currentCmdBuffer_, 0, 1, &vp);

    VkRect2D scissor{{0, 0}, {static_cast<u32>(vp.width), static_cast<u32>(vp.height)}}; // TODO proper values
    vkCmdSetScissor(currentCmdBuffer_, 0, 1, &scissor);
}

void VulkanRenderer::endCamera(Camera *camera, FrameBuffer *renderTarget)
{
    auto &ctx = renderPassContexts_.at(currentRenderPass_);
    ctx.renderPass->end(ctx.cmdBuffer);
    SL_VK_CHECK_RESULT(vkEndCommandBuffer(ctx.cmdBuffer));

    vk::queueSubmit(queue_, 1, &prevSemaphore_, 1, &ctx.completeSemaphore, 1, &ctx.cmdBuffer);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(queue_));

    prevSemaphore_ = ctx.completeSemaphore;

    currentCamera_ = nullptr;
}

void VulkanRenderer::drawMesh(Mesh *mesh, Transform *transform, Material *material)
{
    const auto vkMesh = static_cast<VulkanMesh*>(mesh);
    prepareAndBindMesh(material, transform, mesh);

    if (vkMesh->getPartCount())
    {
        for (u32 part = 0; part < vkMesh->getPartCount(); part++)
        {
            const auto indexBuffer = vkMesh->partBuffer(part);
            vkCmdBindIndexBuffer(currentCmdBuffer_, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
            vkCmdDrawIndexed(currentCmdBuffer_, vkMesh->partIndexElementCount(part), 1, 0, 0, 0);
        }
    }
    else
        vkCmdDraw(currentCmdBuffer_, vkMesh->minVertexCount(), 1, 0, 0);
}

void VulkanRenderer::drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material)
{
    const auto vkMesh = static_cast<VulkanMesh*>(mesh);
    prepareAndBindMesh(material, transform, mesh);
    const auto indexBuffer = vkMesh->partBuffer(part);
    vkCmdBindIndexBuffer(currentCmdBuffer_, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    vkCmdDrawIndexed(currentCmdBuffer_, vkMesh->partIndexElementCount(part), 1, 0, 0, 0);
}

auto VulkanRenderer::ensurePipelineContext(Transform *transform, VulkanMaterial *material, VulkanMesh *mesh) -> PipelineContext&
{
    const auto vkMaterial = static_cast<VulkanMaterial*>(material);
    const auto vkEffect = static_cast<VulkanEffect*>(vkMaterial->getEffect().get());
    const auto vkMesh = static_cast<VulkanMesh*>(mesh);

    const auto key = getPipelineContextKey(transform, currentCamera_, material, *currentRenderPass_);
    auto &context = pipelineContexts_[key];

    if (!context.descSet)
    {
        const auto &uniformBufs = vkEffect->uniformBuffers();
        const auto &effectSamplers = vkEffect->samplers();

        auto builder = VulkanDescriptorSetLayoutBuilder(device_);

        for (const auto &info : uniformBufs)
        {
            const auto bufferName = info.first;
            context.uniformBuffers[bufferName] = VulkanBuffer::createUniformHostVisible(this, info.second.size);
            builder.withBinding(info.second.binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL_GRAPHICS);
        }

        for (auto &pair : effectSamplers)
            builder.withBinding(pair.second.binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);

        context.descSetLayout = builder.build();

        auto poolConfig = VulkanDescriptorPoolConfig();
        if (!uniformBufs.empty())
            poolConfig.forDescriptors(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, static_cast<u32>(uniformBufs.size()));
        if (!effectSamplers.empty())
            poolConfig.forDescriptors(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<u32>(effectSamplers.size()));

        context.descPool = VulkanDescriptorPool(device_, 1, poolConfig);
        context.descSet = context.descPool.allocateSet(context.descSetLayout);
    }

    const auto materialFlagsHash = vkMaterial->stateHash();
    const auto meshLayoutHash = vkMesh->layoutHash();
    const auto materialFlagsChanged = materialFlagsHash != context.lastMaterialFlagsHash;
    const auto meshLayoutChanged = meshLayoutHash != context.lastMeshLayoutHash;

    if (!context.pipeline || materialFlagsChanged || meshLayoutChanged)
    {
        const auto vs = vkEffect->vertexShaderModule();
        const auto fs = vkEffect->fragmentShaderModule();
        auto pipelineConfig = VulkanPipelineConfig(vs, fs)
            .withDescriptorSetLayout(context.descSetLayout)
            .withFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE)
            .withColorBlendAttachmentCount(currentRenderPass_->colorAttachmentCount());
        vkMaterial->configurePipeline(pipelineConfig);

        const auto &effectVertexAttrs = vkEffect->vertexAttributes();
        for (u32 binding = 0; binding < vkMesh->vertexBufferCount(); binding++)
        {
            const auto &layout = vkMesh->vertexBufferLayout(binding);
            
            pipelineConfig.withVertexBinding(binding, layout.getSize(), VK_VERTEX_INPUT_RATE_VERTEX);

            u32 offset = 0;
            for (u32 attrIndex = 0; attrIndex < layout.getAttributeCount(); attrIndex++)
            {
                const auto attr = layout.getAttribute(attrIndex);
                auto format = getVertexAttributeFormat(attr);
                
                auto location = attr.location;
                auto found = true;
                if (!attr.name.empty())
                {
                    if (effectVertexAttrs.count(attr.name))
                        location = effectVertexAttrs.at(attr.name).location;
                    else
                        found = false;
                }
                if (found)
                    pipelineConfig.withVertexAttribute(location, binding, format, offset);

                offset += attr.size;
            }
        }

        context.pipeline = VulkanPipeline{device_, *currentRenderPass_, pipelineConfig};
        context.lastMaterialFlagsHash = materialFlagsHash;
        context.lastMeshLayoutHash = meshLayoutHash;
    }

    return context;
}

void VulkanRenderer::prepareAndBindMesh(Material *material, Transform *transform, Mesh *mesh)
{
    const auto vkMaterial = static_cast<VulkanMaterial*>(material);
    const auto vkEffect = static_cast<VulkanEffect*>(vkMaterial->getEffect().get());
    const auto vkMesh = static_cast<VulkanMesh*>(mesh);
    const auto &uniformBufs = vkEffect->uniformBuffers();
    const auto &materialSamplers = vkMaterial->samplers();
    
    auto &context = ensurePipelineContext(transform, vkMaterial, vkMesh);
    context.frameOfLastUse = frame_;

    // Run the desc set updater
    // TODO Not so often - only when something really changes
    
    VulkanDescriptorSetUpdater updater{device_};

    // TODO Not necessary (?), buffers don't change anyway, only their content
    for (auto &pair : context.uniformBuffers)
    {
        const auto& info = uniformBufs.at(pair.first);
        auto &buffer = pair.second;
        updater.forUniformBuffer(info.binding, context.descSet, buffer, 0, info.size); // TODO use single large buffer?
    }

    for (auto &pair : materialSamplers)
    {
        auto &info = pair.second;
        updater.forImageSampler(
            info.binding,
            context.descSet,
            info.texture->image().view(),
            info.texture->sampler(),
            info.texture->image().layout()
        );
    }

    updater.updateSets();

    // Update buffers content

    auto &bufferItems = vkMaterial->bufferItems();
    for (auto &p: bufferItems)
    {
        auto &buffer = context.uniformBuffers[p.first];
        for (auto &pp: p.second)
            pp.second.write(buffer, currentCamera_, transform);
    }

    // Issue commands

    vkCmdBindPipeline(currentCmdBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, context.pipeline);
    vkCmdBindDescriptorSets(currentCmdBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, context.pipeline.layout(), 0, 1, &context.descSet, 0, nullptr);

    VkDeviceSize vertexBufferOffset = 0;
    for (u32 i = 0; i < vkMesh->vertexBufferCount(); i++)
    {
        auto vertexBuffer = vkMesh->vertexBuffer(i);
        vkCmdBindVertexBuffers(currentCmdBuffer_, i, 1, &vertexBuffer, &vertexBufferOffset);
    }
}

void VulkanRenderer::beginFrame()
{
    frame_++;
    currentCamera_ = nullptr;
    currentRenderPass_ = nullptr;
    currentCmdBuffer_ = nullptr;
    prevSemaphore_ = swapchain_.moveNext();
}

void VulkanRenderer::endFrame()
{
    swapchain_.present(queue_, 1, &prevSemaphore_);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(queue_));

    // Naive cleanup TODO better
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
            const auto frameOfLastUse = p.second.frameOfLastUse;
            if ((frame_ > frameOfLastUse && frame_ - frameOfLastUse >= 100) ||
                (frame_ < frameOfLastUse && frameOfLastUse - frame_ >= 100))
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
            const auto frameOfLastUse = p.second.frameOfLastUse;
            if ((frame_ > frameOfLastUse && frame_ - frameOfLastUse >= 100) ||
                (frame_ < frameOfLastUse && frameOfLastUse - frame_ >= 100))
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
