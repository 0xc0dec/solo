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
#include "platform/opengl/SoloOpenGLCamera.h"
#include "platform/null/SoloNullCamera.h"

using namespace solo;


const uint32_t dirtyBitView = 1;
const uint32_t dirtyBitProjection = 2;
const uint32_t dirtyBitViewProjection = 4;
const uint32_t dirtyBitInvView = 8;
const uint32_t dirtyBitInvViewProjection = 16;


auto Camera::create(const Node &node) -> sptr<Camera>
{
    switch (node.getScene()->getDevice()->getSetup().mode)
    {
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLCamera>(node);
        default:
            return std::make_shared<NullCamera>(node);
    }
}


Camera::Camera(const Node &node):
    ComponentBase(node),
    device(node.getScene()->getDevice()),
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


void Camera::onTransformChanged(const Transform *, uint32_t)
{
    this->dirtyFlags |= dirtyBitView | dirtyBitViewProjection | dirtyBitInvView | dirtyBitInvViewProjection;
}


void Camera::setViewport(float left, float top, float width, float height)
{
    viewport = Vector4(left, top, width, height);
    viewportSet = true;
}


auto Camera::getViewport() const -> Vector4
{
    return viewportSet ? viewport : Vector4();
}


void Camera::setPerspective(bool perspective)
{
    ortho = !perspective;
    dirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


void Camera::setFOV(const Radian &fov)
{
    this->fov = fov;
    dirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


void Camera::setWidth(float width)
{
    this->width = width;
    dirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


void Camera::setHeight(float height)
{
    this->height = height;
    dirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


void Camera::setAspectRatio(float ratio)
{
    aspectRatio = ratio;
    dirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


void Camera::setFar(float far)
{
    this->farClip = far;
    dirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


void Camera::setNear(float near)
{
    this->nearClip = near;
    dirtyFlags |= dirtyBitProjection | dirtyBitViewProjection | dirtyBitInvViewProjection;
}


auto Camera::getViewMatrix() -> const TransformMatrix &
{
    if (dirtyFlags & dirtyBitView)
    {
        viewMatrix = transform->getWorldMatrix();
        viewMatrix.invert();
        dirtyFlags &= ~dirtyBitView;
    }
    return viewMatrix;
}


auto Camera::getInvViewMatrix() -> const TransformMatrix &
{
    if (dirtyFlags & dirtyBitInvView)
    {
        invViewMatrix = getViewMatrix();
        invViewMatrix.invert();
        dirtyFlags &= ~dirtyBitInvView;
    }
    return invViewMatrix;
}


auto Camera::getProjectionMatrix() -> const TransformMatrix &
{
    if (dirtyFlags & dirtyBitProjection)
    {
        if (ortho)
            projectionMatrix = TransformMatrix::createOrthographic(width, height, nearClip, farClip);
        else
            projectionMatrix = TransformMatrix::createPerspective(fov, aspectRatio, nearClip, farClip);
        dirtyFlags &= ~dirtyBitProjection;
    }
    return projectionMatrix;
}


auto Camera::getViewProjectionMatrix() -> const TransformMatrix &
{
    if (dirtyFlags & dirtyBitViewProjection)
    {
        viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
        dirtyFlags &= ~dirtyBitViewProjection;
    }
    return viewProjectionMatrix;
}


auto Camera::getInvViewProjectionMatrix() -> const TransformMatrix &
{
    if (dirtyFlags & dirtyBitInvViewProjection)
    {
        invViewProjectionMatrix = getViewProjectionMatrix();
        invViewProjectionMatrix.invert();
        dirtyFlags &= ~dirtyBitInvViewProjection;
    }
    return invViewProjectionMatrix;
}


void Camera::apply() const
{
    if (renderTarget)
        renderTarget->bind();
    applyImpl();
}


void Camera::finish() const
{
    if (renderTarget)
        renderTarget->unbind();
}
