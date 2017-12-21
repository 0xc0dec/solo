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
#include "SoloRenderCommand.h"
#include "SoloFrameBuffer.h"

using namespace solo;

const u32 ViewDirtyBit = 1;
const u32 ProjectionDirtyBit = 1 << 1;
const u32 ViewProjectionDirtyBit = 1 << 2;
const u32 InvViewDirtyBit = 1 << 3;
const u32 InvViewProjectionDirtyBit = 1 << 4;
const u32 AllProjectionDirtyBits = ProjectionDirtyBit | ViewProjectionDirtyBit | InvViewProjectionDirtyBit;

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
    dirtyFlags |= AllProjectionDirtyBits;
}

void Camera::update()
{
    if (lastTransformVersion != transform->getVersion())
    {
        lastTransformVersion = transform->getVersion();
        dirtyFlags |= ViewDirtyBit | ViewProjectionDirtyBit | InvViewDirtyBit | InvViewProjectionDirtyBit;
    }
}

void Camera::setPerspective(bool perspective)
{
    ortho = !perspective;
    dirtyFlags |= AllProjectionDirtyBits;
}

void Camera::setFOV(const Radian &fov)
{
    this->fov = fov;
    dirtyFlags |= AllProjectionDirtyBits;
}

void Camera::setOrthoSize(const Vector2& size)
{
    orthoSize = size;
    dirtyFlags |= AllProjectionDirtyBits;
}

void Camera::setZFar(float far)
{
    this->zFar = far;
    dirtyFlags |= AllProjectionDirtyBits;
}

void Camera::setZNear(float near)
{
    this->zNear = near;
    dirtyFlags |= AllProjectionDirtyBits;
}

auto Camera::getViewMatrix() const -> const Matrix
{
    if (dirtyFlags & ViewDirtyBit)
    {
        viewMatrix = transform->getWorldMatrix();
        viewMatrix.invert();
        dirtyFlags &= ~ViewDirtyBit;
    }
    return viewMatrix;
}

auto Camera::getInvViewMatrix() const -> const Matrix
{
    if (dirtyFlags & InvViewDirtyBit)
    {
        invViewMatrix = getViewMatrix();
        invViewMatrix.invert();
        dirtyFlags &= ~InvViewDirtyBit;
    }
    return invViewMatrix;
}

auto Camera::getProjectionMatrix() const -> const Matrix
{
    if (dirtyFlags & ProjectionDirtyBit)
    {
        if (ortho)
            projectionMatrix = Matrix::createOrthographic(orthoSize.x, orthoSize.y, zNear, zFar);
        else
            projectionMatrix = Matrix::createPerspective(fov, aspectRatio, zNear, zFar);
        dirtyFlags &= ~ProjectionDirtyBit;
    }
    return projectionMatrix;
}

auto Camera::getViewProjectionMatrix() const -> const Matrix
{
    if (dirtyFlags & ViewProjectionDirtyBit)
    {
        viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
        dirtyFlags &= ~ViewProjectionDirtyBit;
    }
    return viewProjectionMatrix;
}

auto Camera::getInvViewProjectionMatrix() const -> const Matrix
{
    if (dirtyFlags & InvViewProjectionDirtyBit)
    {
        invViewProjectionMatrix = getViewProjectionMatrix();
        invViewProjectionMatrix.invert();
        dirtyFlags &= ~InvViewProjectionDirtyBit;
    }
    return invViewProjectionMatrix;
}

void Camera::renderFrame(std::function<void()> render)
{
    renderer->addRenderCommand(RenderCommand::beginCamera(this, renderTarget.get()));
    render();
    renderer->addRenderCommand(RenderCommand::endCamera(this));
}

auto Camera::canvasPointToWorldRay(const Vector2 &canvasPoint) -> Ray
{
    const auto halfHeightInWorldUnits = zNear * tanf(fov.toRawRadian() / 2);
    const auto halfWidthInWorldUnits = halfHeightInWorldUnits * aspectRatio;
    const auto canvasSize = renderTarget ? renderTarget->getDimensions() : device->getCanvasSize();
    const auto right = transform->getWorldRight() * (halfWidthInWorldUnits * (2 * canvasPoint.x / canvasSize.x - 1));
    const auto down = transform->getWorldDown() * (halfHeightInWorldUnits * (2 * canvasPoint.y / canvasSize.y - 1));
    const auto pos = transform->getWorldPosition();
    const auto canvasCenter = pos + transform->getWorldForward() * zNear;
    const auto origin = canvasCenter + right + down;
    return Ray{origin, (origin - pos).normalized()};
}
