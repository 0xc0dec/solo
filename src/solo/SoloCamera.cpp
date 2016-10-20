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

using namespace solo;


const uint32_t DirtyBitView = 1;
const uint32_t DirtyBitProjection = 2;
const uint32_t DirtyBitViewProjection = 4;
const uint32_t DirtyBitInvView = 8;
const uint32_t DirtyBitInvViewProjection = 16;


Camera::Camera(const Node& node):
    ComponentBase(node),
    device(Device::get()),
    renderer(Device::get()->getRenderer()),
    fov(Degree(60))
{
    renderQueue = KnownRenderQueues::Camera;
}


void Camera::init()
{
    transform = node.findComponent<Transform>();
    transform->addCallback(this);
    auto canvasSize = device->getCanvasSize();
    setAspectRatio(canvasSize.x / canvasSize.y);
}


void Camera::onTransformChanged(const Transform*, uint32_t)
{
    this->dirtyFlags |= DirtyBitView | DirtyBitViewProjection | DirtyBitInvView | DirtyBitInvViewProjection;
}


void Camera::setViewport(float left, float top, float width, float height)
{
    viewport = Vector4(left, top, width, height);
    viewportSet = true;
}


auto Camera::getViewport() const -> Vector4
{
    if (viewportSet)
        return viewport;
    return Vector4();
}


void Camera::setPerspective(bool perspective)
{
    ortho = !perspective;
    dirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


void Camera::setFOV(const Radian& fov)
{
    this->fov = fov;
    dirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


void Camera::setWidth(float width)
{
    this->width = width;
    dirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


void Camera::setHeight(float height)
{
    this->height = height;
    dirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


void Camera::setAspectRatio(float ratio)
{
    aspectRatio = ratio;
    dirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


void Camera::setFar(float far)
{
    this->farClip = far;
    dirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


void Camera::setNear(float near)
{
    this->nearClip = near;
    dirtyFlags |= DirtyBitProjection | DirtyBitViewProjection | DirtyBitInvViewProjection;
}


auto Camera::getViewMatrix() -> const TransformMatrix&
{
    if (dirtyFlags & DirtyBitView)
    {
        viewMatrix = transform->getWorldMatrix();
        viewMatrix.invert();
        dirtyFlags &= ~DirtyBitView;
    }
    return viewMatrix;
}


auto Camera::getInvViewMatrix() -> const TransformMatrix&
{
    if (dirtyFlags & DirtyBitInvView)
    {
        invViewMatrix = getViewMatrix();
        invViewMatrix.invert();
        dirtyFlags &= ~DirtyBitInvView;
    }
    return invViewMatrix;
}


auto Camera::getProjectionMatrix() -> const TransformMatrix&
{
    if (dirtyFlags & DirtyBitProjection)
    {
        if (ortho)
            projectionMatrix = TransformMatrix::createOrthographic(width, height, nearClip, farClip);
        else
            projectionMatrix = TransformMatrix::createPerspective(fov, aspectRatio, nearClip, farClip);
        dirtyFlags &= ~DirtyBitProjection;
    }
    return projectionMatrix;
}


auto Camera::getViewProjectionMatrix() -> const TransformMatrix&
{
    if (dirtyFlags & DirtyBitViewProjection)
    {
        viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
        dirtyFlags &= ~DirtyBitViewProjection;
    }
    return viewProjectionMatrix;
}


auto Camera::getInvViewProjectionMatrix() -> const TransformMatrix&
{
    if (dirtyFlags & DirtyBitInvViewProjection)
    {
        invViewProjectionMatrix = getViewProjectionMatrix();
        invViewProjectionMatrix.invert();
        dirtyFlags &= ~DirtyBitInvViewProjection;
    }
    return invViewProjectionMatrix;
}


void Camera::apply() const
{
    if (renderTarget)
        renderTarget->bind();

    if (viewportSet)
    {
        renderer->setViewport(
            static_cast<uint32_t>(viewport.x),
            static_cast<uint32_t>(viewport.y),
            static_cast<uint32_t>(viewport.z),
            static_cast<uint32_t>(viewport.w));
    }
    else
    {
        auto size = device->getCanvasSize();
        renderer->setViewport(0, 0, static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y));
    }

    renderer->setDepthWrite(true);
    renderer->setDepthTest(true);
    renderer->clear(true, true, clearColor.x, clearColor.y, clearColor.z, clearColor.w);
}


void Camera::finish() const
{
    if (renderTarget)
        renderTarget->unbind();
}
