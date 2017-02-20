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

#include "SoloCamera.h"
#include "SoloNode.h"
#include "SoloDevice.h"
#include "SoloFrameBuffer.h"
#include "SoloRadian.h"
#include "SoloDegree.h"
#include "SoloScene.h"
#include "SoloRenderContext.h"
#include "platform/opengl/SoloOpenGLCamera.h"
#include "platform/vulkan/SoloVulkanCamera.h"
#include "platform/null/SoloNullCamera.h"

using namespace solo;


const uint32_t DirtyBitView = 1;
const uint32_t DirtyBitProjection = 1 << 1;
const uint32_t DirtyBitViewProjection = 1 << 2;
const uint32_t DirtyBitInvView = 1 << 3;
const uint32_t DirtyBitInvViewProjection = 1 << 4;


auto Camera::create(const Node &node) -> sptr<Camera>
{
    switch (node.getScene()->getDevice()->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Camera>(node);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<vk::Camera>(node);
#endif
        default:
            return std::make_shared<NullCamera>(node);
    }
}


Camera::Camera(const Node &node):
    ComponentBase(node),
    device(node.getScene()->getDevice()),
    fov(Degree(60))
{
}


void Camera::init()
{
    transform = node.findComponent<Transform>();
    transform->addCallback(this);
    auto canvasSize = device->getCanvasSize();
    setAspectRatio(canvasSize.x / canvasSize.y);
}


void Camera::onTransformChanged(const Transform *, uint32_t)
{
    this->transformDirtyFlags |= DirtyBitView | DirtyBitViewProjection | DirtyBitInvView | DirtyBitInvViewProjection;
}


void Camera::setPerspective(bool perspective)
{
    ortho = !perspective;
    transformDirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


void Camera::setFOV(const Radian &fov)
{
    this->fov = fov;
    transformDirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


void Camera::setWidth(float width)
{
    this->width = width;
    transformDirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


void Camera::setHeight(float height)
{
    this->height = height;
    transformDirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


void Camera::setAspectRatio(float ratio)
{
    aspectRatio = ratio;
    transformDirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


void Camera::setFar(float far)
{
    this->farClip = far;
    transformDirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


void Camera::setNear(float near)
{
    this->nearClip = near;
    transformDirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


auto Camera::getViewMatrix() const -> const TransformMatrix
{
    if (transformDirtyFlags & DirtyBitView)
    {
        viewMatrix = transform->getWorldMatrix();
        viewMatrix.invert();
        transformDirtyFlags &= ~DirtyBitView;
    }
    return viewMatrix;
}


auto Camera::getInvViewMatrix() const -> const TransformMatrix
{
    if (transformDirtyFlags & DirtyBitInvView)
    {
        invViewMatrix = getViewMatrix();
        invViewMatrix.invert();
        transformDirtyFlags &= ~DirtyBitInvView;
    }
    return invViewMatrix;
}


auto Camera::getProjectionMatrix() const -> const TransformMatrix
{
    if (transformDirtyFlags & DirtyBitProjection)
    {
        if (ortho)
            projectionMatrix = TransformMatrix::createOrthographic(width, height, nearClip, farClip);
        else
            projectionMatrix = TransformMatrix::createPerspective(fov, aspectRatio, nearClip, farClip);
        transformDirtyFlags &= ~DirtyBitProjection;
    }
    return projectionMatrix;
}


auto Camera::getViewProjectionMatrix() const -> const TransformMatrix
{
    if (transformDirtyFlags & DirtyBitViewProjection)
    {
        viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
        transformDirtyFlags &= ~DirtyBitViewProjection;
    }
    return viewProjectionMatrix;
}


auto Camera::getInvViewProjectionMatrix() const -> const TransformMatrix
{
    if (transformDirtyFlags & DirtyBitInvViewProjection)
    {
        invViewProjectionMatrix = getViewProjectionMatrix();
        invViewProjectionMatrix.invert();
        transformDirtyFlags &= ~DirtyBitInvViewProjection;
    }
    return invViewProjectionMatrix;
}


void Camera::renderFrame(std::function<void(const RenderContext&)> render) const
{
    if (renderTarget)
        renderTarget->bind();
    
    renderImpl();

    RenderContext ctx;
    ctx.camera = this;
    render(ctx);

    if (renderTarget)
        renderTarget->unbind();
}
