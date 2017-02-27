/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanCamera.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkanRenderer.h"
#include "SoloVulkanRenderCommand.h"

using namespace solo;


vk::Camera::Camera(const Node &node):
    solo::Camera(node)
{
    renderer = dynamic_cast<Renderer*>(node.getScene()->getDevice()->getRenderer());
}


//void vk::Camera::beginFrame() const
//{
//    renderer->addRenderCommand(RenderCommand::beginCamera(this));
//}
//
//
//void vk::Camera::endFrame() const
//{
//    renderer->addRenderCommand(RenderCommand::endCamera());
//}


#endif
