/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloCamera.h"
#include "SoloNode.h"
#include "SoloDevice.h"
#include "SoloFrameBuffer.h"
#include "SoloRadian.h"
#include "SoloDegree.h"
#include "SoloScene.h"
#include "SoloRenderer.h"

using namespace solo;


const uint32_t DirtyBitView = 1;
const uint32_t DirtyBitProjection = 1 << 1;
const uint32_t DirtyBitViewProjection = 1 << 2;
const uint32_t DirtyBitInvView = 1 << 3;
const uint32_t DirtyBitInvViewProjection = 1 << 4;


auto Camera::create(const Node &node) -> sptr<Camera>
{
    return std::shared_ptr<Camera>(new Camera(node));
}


Camera::Camera(const Node &node):
    ComponentBase(node),
    device(node.getScene()->getDevice()),
    renderer(device->getRenderer()),
    fov(Degree(60))
{
    auto canvasSize = device->getCanvasSize();
    viewport = {0, 0, canvasSize.x, canvasSize.y};
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


void Camera::renderFrame(std::function<void()> render) const
{
    renderer->addRenderCommand(RenderCommand::beginCamera(this, renderTarget.get()));
    render();
    renderer->addRenderCommand(RenderCommand::endCamera());
}
