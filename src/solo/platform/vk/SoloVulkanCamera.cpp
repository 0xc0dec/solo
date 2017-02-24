/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanCamera.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkanRenderer.h"

using namespace solo;


vk::Camera::Camera(const Node &node):
    solo::Camera(node)
{
    renderer = dynamic_cast<Renderer*>(node.getScene()->getDevice()->getRenderer());
}


void vk::Camera::beginFrame() const
{
    renderer->beginCamera(this);
}


void vk::Camera::endFrame() const
{
    renderer->endCamera();
}


#endif
