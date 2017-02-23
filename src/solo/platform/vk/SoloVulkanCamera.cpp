/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloVulkanCamera.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkanRenderer.h"

using namespace solo;
using namespace vk;


vk::Camera::Camera(const Node &node):
    solo::Camera(node)
{
    renderer = dynamic_cast<Renderer*>(node.getScene()->getDevice()->getRenderer());
}


void vk::Camera::renderImpl() const
{
    auto canvasSize = device->getCanvasSize(); // TODO cache

    renderer->beginRenderPass(canvasSize.x, canvasSize.y,
        clearFlags.color, clearFlags.depth, clearColor,
        viewport.x >= 0 ? viewport : Vector4{0, 0, canvasSize.x, canvasSize.y});
    
    renderer->endRenderPass();
}


#endif
