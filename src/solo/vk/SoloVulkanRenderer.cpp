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

static auto getPipelineContextKey(Transform *transform, Camera *camera, VulkanMaterial *material)
{
    size_t seed = 0;
    const std::hash<void*> hasher;
    combineHash(seed, hasher(material));
    combineHash(seed, hasher(camera));
    combineHash(seed, hasher(transform));
    return seed;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackFunc(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
    u64 obj, size_t location, s32 code, const s8 *layerPrefix, const s8 *msg, void *userData)
{
	Logger::global().logDebug(SL_FMT("Vulkan debug message: ", msg));
    return VK_FALSE;
}

static auto createDebugCallback(const VkInstance instance, const PFN_vkDebugReportCallbackEXT callbackFunc) -> VulkanResource<VkDebugReportCallbackEXT>
{
    VkDebugReportCallbackCreateInfoEXT createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = callbackFunc;

	const auto create = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
    panicIf(!create, "Failed to load pointer to vkCreateDebugReportCallbackEXT");

	const auto destroy = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
    panicIf(!destroy, "Failed to load pointer to vkDestroyDebugReportCallbackEXT");

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

    VkDeviceCreateInfo deviceCreateInfo {};
    vec<VkPhysicalDeviceFeatures> enabledFeatures {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.pEnabledFeatures = enabledFeatures.data();
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

    return panic<u32>("Could not find queue index");
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
    // Since all depth formats may be optional, we need to find a suitable depth format to use
    // Start with the highest precision packed format
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
        VkFormatProperties formatProps;
        vkGetPhysicalDeviceFormatProperties(device, format, &formatProps);
        // Format must support depth stencil attachment for optimal tiling
        if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
            return format;
    }

    return VK_FORMAT_UNDEFINED;
}

VulkanRenderer::VulkanRenderer(Device *engineDevice):
    engineDevice(engineDevice)
{
    const auto vulkanDevice = dynamic_cast<VulkanSDLDevice*>(engineDevice);
    const auto instance = vulkanDevice->getInstance();
    const auto surface = vulkanDevice->getSurface();
    const auto canvasSize = engineDevice->getCanvasSize();

#ifdef SL_DEBUG
    debugCallback = createDebugCallback(instance, debugCallbackFunc);
#endif
    physicalDevice = ::getPhysicalDevice(instance);
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &physicalFeatures);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalMemoryFeatures);

    auto surfaceFormats = getSurfaceFormats(physicalDevice, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);
    depthFormat = ::getDepthFormat(physicalDevice);

    const auto queueIndex = getQueueIndex(physicalDevice, surface);
    device = createDevice(physicalDevice, queueIndex);
    vkGetDeviceQueue(device, queueIndex, 0, &queue);

    commandPool = createCommandPool(device, queueIndex);
    swapchain = VulkanSwapchain(this, vulkanDevice, canvasSize.x, canvasSize.y, engineDevice->isVsync());
}

void VulkanRenderer::beginCamera(Camera *camera, FrameBuffer *renderTarget)
{
    currentCamera = camera;
    currentRenderPass = &swapchain.getRenderPass();
    const auto canvasSize = engineDevice->getCanvasSize();
    auto currentFrameBuffer = swapchain.getCurrentFrameBuffer();
    auto dimensions = canvasSize;
    vec<VkClearAttachment> clearAttachments = {{VK_IMAGE_ASPECT_COLOR_BIT, 0, {{0, 0, 0, 1}}}}; // TODO avoid mem allocation

    if (renderTarget)
    {
        const auto targetFrameBuffer = static_cast<VulkanFrameBuffer*>(renderTarget);
        currentRenderPass = &targetFrameBuffer->getRenderPass();
        currentFrameBuffer = targetFrameBuffer->getHandle();
        dimensions = targetFrameBuffer->getDimensions();
        clearAttachments.resize(targetFrameBuffer->getColorAttachmentCount());
    }

    if (!renderPassContexts.count(currentRenderPass))
    {
        // TODO Clear later
        renderPassContexts[currentRenderPass].cmdBuffer = vk::createCommandBuffer(device, commandPool);
        renderPassContexts[currentRenderPass].completeSemaphore = vk::createSemaphore(device);
    }

    auto &passContext = renderPassContexts.at(currentRenderPass);
    currentCmdBuffer = passContext.cmdBuffer;

    if (!passContext.started)
    {
        passContext.started = true;
        passesToRender.push_back(currentRenderPass);

        vk::beginCommandBuffer(currentCmdBuffer, false);

        currentRenderPass->begin(
            currentCmdBuffer,
            currentFrameBuffer,
            dimensions.x, dimensions.y
        );

        if (currentCamera->hasColorClearing())
        {
            VkClearRect clearRect{};
            clearRect.layerCount = 1;
            clearRect.rect.offset = {0, 0};
            clearRect.rect.extent = {static_cast<u32>(dimensions.x), static_cast<u32>(dimensions.y)};
            auto clearColor = currentCamera->getClearColor();
            for (u32 i = 0; i < clearAttachments.size(); ++i)
            {
                clearAttachments[i].aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                clearAttachments[i].colorAttachment = i;
                clearAttachments[i].clearValue = {{clearColor.x, clearColor.y, clearColor.z, clearColor.w}};
            }
            vkCmdClearAttachments(currentCmdBuffer, clearAttachments.size(), clearAttachments.data(), 1, &clearRect);
        }
    }

    const auto viewport = currentCamera->getViewport();
    VkViewport vp{viewport.x, viewport.y, viewport.z, viewport.w, 0, 1};
    vkCmdSetViewport(currentCmdBuffer, 0, 1, &vp);

    VkRect2D scissor{{0, 0}, {vp.width, vp.height}}; // TODO proper values
    vkCmdSetScissor(currentCmdBuffer, 0, 1, &scissor);
}

void VulkanRenderer::endCamera(Camera *camera, FrameBuffer *renderTarget)
{
    currentCamera = nullptr;
}

void VulkanRenderer::drawMesh(Mesh *mesh, Transform *transform, Material *material)
{
    if (currentCamera)
        drawMesh(currentCmdBuffer, *currentRenderPass, material, transform, mesh, currentCamera);
}

void VulkanRenderer::drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material)
{
    if (currentCamera)
        drawMeshPart(currentCmdBuffer, *currentRenderPass, material, transform, mesh, currentCamera, part);
}

auto VulkanRenderer::ensurePipelineContext(Transform *transform, Camera *camera, VulkanMaterial *material,
    VulkanMesh *mesh, VkRenderPass renderPass) -> PipelineContext&
{
    const auto vkMaterial = static_cast<VulkanMaterial*>(material);
    const auto vkEffect = static_cast<VulkanEffect*>(vkMaterial->getEffect().get());
    const auto vkMesh = static_cast<VulkanMesh*>(mesh);

	const auto key = getPipelineContextKey(transform, camera, material);
    auto &context = pipelineContexts[key];

    if (!context.descSet)
    {
        const auto &uniformBufs = vkEffect->getUniformBuffers();
        const auto &effectSamplers = vkEffect->getSamplers();

        auto builder = VulkanDescriptorSetLayoutBuilder(device);

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
        poolConfig.forDescriptors(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uniformBufs.size());
        poolConfig.forDescriptors(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, effectSamplers.size());

        context.descPool = VulkanDescriptorPool(device, 1, poolConfig);
        context.descSet = context.descPool.allocateSet(context.descSetLayout);
    }

    const auto materialFlagsHash = vkMaterial->getStateHash();
    const auto meshLayoutHash = vkMesh->getLayoutHash();
    const auto materialFlagsChanged = materialFlagsHash != context.lastMaterialFlagsHash;
    const auto meshLayoutChanged = meshLayoutHash != context.lastMeshLayoutHash;

    if (!context.pipeline || materialFlagsChanged || meshLayoutChanged)
    {
        const auto vs = vkEffect->getVkVertexShader();
        const auto fs = vkEffect->getVkFragmentShader();
        auto pipelineConfig = VulkanPipelineConfig(vs, fs)
            .withDescriptorSetLayout(context.descSetLayout)
            .withFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
        vkMaterial->configurePipeline(pipelineConfig);

        const auto &effectVertexAttrs = vkEffect->getVertexAttributes();
        for (s32 binding = 0; binding < vkMesh->getVertexBufferCount(); binding++)
        {
            const auto &layout = vkMesh->getVertexBufferLayout(binding);
            
            pipelineConfig.withVertexBinding(binding, layout.getSize(), VK_VERTEX_INPUT_RATE_VERTEX);

			u32 offset = 0;
            for (auto attrIndex = 0; attrIndex < layout.getAttributeCount(); attrIndex++)
            {
                const auto attr = layout.getAttribute(attrIndex);

                VkFormat format;
                switch (attr.elementCount)
                {
                    case 1:
                        format = VK_FORMAT_R32_SFLOAT;
                        break;
                    case 2:
                        format = VK_FORMAT_R32G32_SFLOAT;
                        break;
                    case 3:
                        format = VK_FORMAT_R32G32B32_SFLOAT;
                        break;
                    case 4:
                        format = VK_FORMAT_R32G32B32A32_SFLOAT;
                        break;
                    default:
                        panic("Unsupported vertex attribute element count");
                        break;
                }
                
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

        context.pipeline = VulkanPipeline{device, renderPass, pipelineConfig};
        context.lastMaterialFlagsHash = materialFlagsHash;
        context.lastMeshLayoutHash = meshLayoutHash;
    }

    return context;
}

void VulkanRenderer::drawMeshPart(
    VkCommandBuffer cmdBuf, VkRenderPass renderPass, Material *material,
    Transform *transform, Mesh *mesh, Camera *camera, u32 part
)
{
    const auto vkMesh = static_cast<VulkanMesh*>(mesh);
    prepareAndBindMesh(cmdBuf, renderPass, material, transform, mesh, camera);
    const auto indexBuffer = vkMesh->getPartBuffer(part);
    vkCmdBindIndexBuffer(cmdBuf, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    vkCmdDrawIndexed(cmdBuf, vkMesh->getPartIndexElementCount(part), 1, 0, 0, 0);
}

void VulkanRenderer::drawMesh(
    VkCommandBuffer cmdBuf, VkRenderPass renderPass, Material *material,
    Transform *transform, Mesh *mesh, Camera *camera
)
{
    const auto vkMesh = static_cast<VulkanMesh*>(mesh);
    prepareAndBindMesh(cmdBuf, renderPass, material, transform, mesh, camera);

    if (vkMesh->getPartCount())
    {
        for (u32 part = 0; part < vkMesh->getPartCount(); part++)
        {
            const auto indexBuffer = vkMesh->getPartBuffer(part);
            vkCmdBindIndexBuffer(cmdBuf, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
            vkCmdDrawIndexed(cmdBuf, vkMesh->getPartIndexElementCount(part), 1, 0, 0, 0);
        }
    }
    else
        vkCmdDraw(cmdBuf, vkMesh->getMinVertexCount(), 1, 0, 0);
}

void VulkanRenderer::prepareAndBindMesh(
    VkCommandBuffer cmdBuf, VkRenderPass renderPass, Material *material,
    Transform *transform, Mesh *mesh, Camera *camera)
{
    const auto vkMaterial = static_cast<VulkanMaterial*>(material);
    const auto vkEffect = static_cast<VulkanEffect*>(vkMaterial->getEffect().get());
    const auto vkMesh = static_cast<VulkanMesh*>(mesh);
    const auto &uniformBufs = vkEffect->getUniformBuffers();
    const auto &materialSamplers = vkMaterial->getSamplers();
    
    auto &context = ensurePipelineContext(transform, camera, vkMaterial, vkMesh, renderPass);
    
    // Run the desc set updater
    // TODO Not so often - only when something really changes
    
    VulkanDescriptorSetUpdater updater{device};

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
            info.texture->getImage().getView(),
            info.texture->getSampler(),
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );
    }

    updater.updateSets();

    // Update buffers content

    auto &bufferItems = vkMaterial->getBufferItems();
    for (auto &p: bufferItems)
    {
        auto &buffer = context.uniformBuffers[p.first];
        for (auto &pp: p.second)
            pp.second.write(buffer, camera, transform);
    }

    // Issue commands

    vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, context.pipeline);
    vkCmdBindDescriptorSets(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, context.pipeline.getLayout(), 0, 1, &context.descSet, 0, nullptr);

    VkDeviceSize vertexBufferOffset = 0;
    for (u32 i = 0; i < vkMesh->getVertexBufferCount(); i++)
    {
        auto vertexBuffer = vkMesh->getVertexBuffer(i);
        vkCmdBindVertexBuffers(cmdBuf, i, 1, &vertexBuffer, &vertexBufferOffset);
    }
}

void VulkanRenderer::beginFrame()
{
    currentCamera = nullptr;
    currentRenderPass = nullptr;
    currentCmdBuffer = nullptr;
    passesToRender.clear();
}

void VulkanRenderer::endFrame()
{
    for (auto &pair: renderPassContexts)
    {
        if (!pair.second.started)
            continue;
        
        pair.second.started = false;
	    const VkCommandBuffer cmdBuffer = pair.second.cmdBuffer;
        pair.first->end(cmdBuffer);
        SL_VK_CHECK_RESULT(vkEndCommandBuffer(cmdBuffer));
    }

    VkSemaphore prevSemaphore = swapchain.moveNext();
    for (auto &pass: passesToRender)
    {
        auto &ctx = renderPassContexts.at(pass);
        vk::queueSubmit(queue, 1, &prevSemaphore, 1, &ctx.completeSemaphore, 1, &ctx.cmdBuffer);
        prevSemaphore = ctx.completeSemaphore;
    }

    swapchain.present(queue, 1, &prevSemaphore);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(queue));
}

#endif
