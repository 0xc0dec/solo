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


const uint32_t dirtyBitView = 1;
const uint32_t dirtyBitProjection = 1 << 1;
const uint32_t dirtyBitViewProjection = 1 << 2;
const uint32_t dirtyBitInvView = 1 << 3;
const uint32_t dirtyBitInvViewProjection = 1 << 4;


auto Camera::create(const Node &node) -> sptr<Camera>
{
    switch (node.getScene()->getDevice()->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLCamera>(node);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanCamera>(node);
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
    this->transformDirtyFlags |= dirtyBitView | dirtyBitViewProjection | dirtyBitInvView | dirtyBitInvViewProjection;
}


void Camera::setPerspective(bool perspective)
{
    ortho = !perspective;
    transformDirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


void Camera::setFOV(const Radian &fov)
{
    this->fov = fov;
    transformDirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


void Camera::setWidth(float width)
{
    this->width = width;
    transformDirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


void Camera::setHeight(float height)
{
    this->height = height;
    transformDirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


void Camera::setAspectRatio(float ratio)
{
    aspectRatio = ratio;
    transformDirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


void Camera::setFar(float far)
{
    this->farClip = far;
    transformDirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


void Camera::setNear(float near)
{
    this->nearClip = near;
    transformDirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


auto Camera::getViewMatrix() const -> const TransformMatrix &
{
    if (transformDirtyFlags & dirtyBitView)
    {
        viewMatrix = transform->getWorldMatrix();
        viewMatrix.invert();
        transformDirtyFlags &= ~dirtyBitView;
    }
    return viewMatrix;
}


auto Camera::getInvViewMatrix() const -> const TransformMatrix &
{
    if (transformDirtyFlags & dirtyBitInvView)
    {
        invViewMatrix = getViewMatrix();
        invViewMatrix.invert();
        transformDirtyFlags &= ~dirtyBitInvView;
    }
    return invViewMatrix;
}


auto Camera::getProjectionMatrix() const -> const TransformMatrix &
{
    if (transformDirtyFlags & dirtyBitProjection)
    {
        if (ortho)
            projectionMatrix = TransformMatrix::createOrthographic(width, height, nearClip, farClip);
        else
            projectionMatrix = TransformMatrix::createPerspective(fov, aspectRatio, nearClip, farClip);
        transformDirtyFlags &= ~dirtyBitProjection;
    }
    return projectionMatrix;
}


auto Camera::getViewProjectionMatrix() const -> const TransformMatrix &
{
    if (transformDirtyFlags & dirtyBitViewProjection)
    {
        viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
        transformDirtyFlags &= ~dirtyBitViewProjection;
    }
    return viewProjectionMatrix;
}


auto Camera::getInvViewProjectionMatrix() const -> const TransformMatrix &
{
    if (transformDirtyFlags & dirtyBitInvViewProjection)
    {
        invViewProjectionMatrix = getViewProjectionMatrix();
        invViewProjectionMatrix.invert();
        transformDirtyFlags &= ~dirtyBitInvViewProjection;
    }
    return invViewProjectionMatrix;
}


void Camera::render(std::function<void(const RenderContext&)> render) const
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
