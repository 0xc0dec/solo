/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloCamera.h"
#include "SoloNode.h"
#include "SoloDevice.h"
#include "SoloRadian.h"
#include "SoloDegree.h"
#include "SoloScene.h"
#include "SoloRay.h"
#include "SoloRenderer.h"

using namespace solo;

const u32 viewDirtyBit = 1;
const u32 projectionDirtyBit = 1 << 1;
const u32 viewProjectionDirtyBit = 1 << 2;
const u32 invViewDirtyBit = 1 << 3;
const u32 invViewProjectionDirtyBit = 1 << 4;
const u32 allProjectionDirtyBits = projectionDirtyBit | viewProjectionDirtyBit | invViewProjectionDirtyBit;

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
    const auto canvasSize = device->getCanvasSize();
    aspectRatio = canvasSize.x / canvasSize.y;
    dirtyFlags |= allProjectionDirtyBits;
}

void Camera::update()
{
    if (lastTransformVersion != transform->getVersion())
    {
        lastTransformVersion = transform->getVersion();
        dirtyFlags |= viewDirtyBit | viewProjectionDirtyBit | invViewDirtyBit | invViewProjectionDirtyBit;
    }
}

void Camera::setPerspective(bool perspective)
{
    ortho = !perspective;
    dirtyFlags |= allProjectionDirtyBits;
}

void Camera::setFOV(const Radian &fov)
{
    this->fov = fov;
    dirtyFlags |= allProjectionDirtyBits;
}

void Camera::setOrthoSize(const Vector2& size)
{
    orthoSize = size;
    dirtyFlags |= allProjectionDirtyBits;
}

void Camera::setZFar(float far)
{
    this->zFar = far;
    dirtyFlags |= allProjectionDirtyBits;
}

void Camera::setZNear(float near)
{
    this->zNear = near;
    dirtyFlags |= allProjectionDirtyBits;
}

auto Camera::getViewMatrix() const -> const Matrix
{
    if (dirtyFlags & viewDirtyBit)
    {
        viewMatrix = transform->getWorldMatrix();
        viewMatrix.invert();
        dirtyFlags &= ~viewDirtyBit;
    }
    return viewMatrix;
}

auto Camera::getInvViewMatrix() const -> const Matrix
{
    if (dirtyFlags & invViewDirtyBit)
    {
        invViewMatrix = getViewMatrix();
        invViewMatrix.invert();
        dirtyFlags &= ~invViewDirtyBit;
    }
    return invViewMatrix;
}

auto Camera::getProjectionMatrix() const -> const Matrix
{
    if (dirtyFlags & projectionDirtyBit)
    {
        if (ortho)
            projectionMatrix = Matrix::createOrthographic(orthoSize.x, orthoSize.y, zNear, zFar);
        else
            projectionMatrix = Matrix::createPerspective(fov, aspectRatio, zNear, zFar);
        dirtyFlags &= ~projectionDirtyBit;
    }
    return projectionMatrix;
}

auto Camera::getViewProjectionMatrix() const -> const Matrix
{
    if (dirtyFlags & viewProjectionDirtyBit)
    {
        viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
        dirtyFlags &= ~viewProjectionDirtyBit;
    }
    return viewProjectionMatrix;
}

auto Camera::getInvViewProjectionMatrix() const -> const Matrix
{
    if (dirtyFlags & invViewProjectionDirtyBit)
    {
        invViewProjectionMatrix = getViewProjectionMatrix();
        invViewProjectionMatrix.invert();
        dirtyFlags &= ~invViewProjectionDirtyBit;
    }
    return invViewProjectionMatrix;
}

void Camera::renderFrame(const std::function<void()> &render)
{
    renderer->beginCamera(this, renderTarget.get());
    render();
    renderer->endCamera(this, renderTarget.get());
}

auto Camera::windowPointToWorldRay(const Vector2 &pt) const -> Ray
{
    const auto halfHeightInWorldUnits = zNear * std::tan(fov.toRawRadian() / 2);
    const auto halfWidthInWorldUnits = halfHeightInWorldUnits * aspectRatio;
    const auto canvasSize = device->getDpiIndependentCanvasSize();
    const auto right = transform->getWorldRight() * (halfWidthInWorldUnits * (2 * pt.x / canvasSize.x - 1));
    const auto down = transform->getWorldDown() * (halfHeightInWorldUnits * (2 * pt.y / canvasSize.y - 1));
    const auto pos = transform->getWorldPosition();
    const auto canvasCenter = pos + transform->getWorldForward() * zNear;
    const auto origin = canvasCenter + right + down;
    return Ray{origin, (origin - pos).normalized()};
}
