/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloCamera.h"
#include "SoloNode.h"
#include "SoloDevice.h"
#include "SoloRadians.h"
#include "SoloDegrees.h"
#include "SoloScene.h"
#include "SoloRay.h"
#include "SoloRenderer.h"

using namespace solo;

static const u32 ViewDirtyBit = 1;
static const u32 ProjectionDirtyBit = 1 << 1;
static const u32 ViewProjectionDirtyBit = 1 << 2;
static const u32 InvViewDirtyBit = 1 << 3;
static const u32 InvViewProjectionDirtyBit = 1 << 4;
static const u32 AllProjectionDirtyBits = ProjectionDirtyBit | ViewProjectionDirtyBit | InvViewProjectionDirtyBit;

auto Camera::create(const Node &node) -> sptr<Camera>
{
    return std::shared_ptr<Camera>(new Camera(node));
}

Camera::Camera(const Node &node):
    ComponentBase(node),
    device_(node.getScene()->getDevice()),
    renderer_(device_->getRenderer()),
    fov_(Degrees(60))
{
    auto canvasSize = device_->getCanvasSize();
    viewport_ = Vector4(0, 0, canvasSize.x(), canvasSize.y());
}

void Camera::init()
{
    transform_ = node_.findComponent<Transform>();
    const auto canvasSize = device_->getCanvasSize();
    aspectRatio_ = canvasSize.x() / canvasSize.y();
    dirtyFlags_ |= AllProjectionDirtyBits;
}

void Camera::update()
{
    if (lastTransformVersion_ != transform_->getVersion())
    {
        lastTransformVersion_ = transform_->getVersion();
        dirtyFlags_ |= ViewDirtyBit | ViewProjectionDirtyBit | InvViewDirtyBit | InvViewProjectionDirtyBit;
    }
}

void Camera::setPerspective(bool perspective)
{
    ortho_ = !perspective;
    dirtyFlags_ |= AllProjectionDirtyBits;
}

void Camera::setFOV(const Radians &fov)
{
    this->fov_ = fov;
    dirtyFlags_ |= AllProjectionDirtyBits;
}

void Camera::setOrthoSize(const Vector2& size)
{
    orthoSize_ = size;
    dirtyFlags_ |= AllProjectionDirtyBits;
}

void Camera::setZFar(float far)
{
    this->zFar_ = far;
    dirtyFlags_ |= AllProjectionDirtyBits;
}

void Camera::setZNear(float near)
{
    this->zNear_ = near;
    dirtyFlags_ |= AllProjectionDirtyBits;
}

auto Camera::getViewMatrix() const -> Matrix
{
    if (dirtyFlags_ & ViewDirtyBit)
    {
        viewMatrix_ = transform_->getWorldMatrix();
        viewMatrix_.invert();
        dirtyFlags_ &= ~ViewDirtyBit;
    }
    return viewMatrix_;
}

auto Camera::getInvViewMatrix() const -> Matrix
{
    if (dirtyFlags_ & InvViewDirtyBit)
    {
        invViewMatrix_ = getViewMatrix();
        invViewMatrix_.invert();
        dirtyFlags_ &= ~InvViewDirtyBit;
    }
    return invViewMatrix_;
}

auto Camera::getProjectionMatrix() const -> Matrix
{
    if (dirtyFlags_ & ProjectionDirtyBit)
    {
        if (ortho_)
            projectionMatrix_ = Matrix::createOrthographic(orthoSize_.x(), orthoSize_.y(), zNear_, zFar_);
        else
            projectionMatrix_ = Matrix::createPerspective(fov_, aspectRatio_, zNear_, zFar_);
        dirtyFlags_ &= ~ProjectionDirtyBit;
    }
    return projectionMatrix_;
}

auto Camera::getViewProjectionMatrix() const -> Matrix
{
    if (dirtyFlags_ & ViewProjectionDirtyBit)
    {
        viewProjectionMatrix_ = getProjectionMatrix() * getViewMatrix();
        dirtyFlags_ &= ~ViewProjectionDirtyBit;
    }
    return viewProjectionMatrix_;
}

auto Camera::getInvViewProjectionMatrix() const -> Matrix
{
    if (dirtyFlags_ & InvViewProjectionDirtyBit)
    {
        invViewProjectionMatrix_ = getViewProjectionMatrix();
        invViewProjectionMatrix_.invert();
        dirtyFlags_ &= ~InvViewProjectionDirtyBit;
    }
    return invViewProjectionMatrix_;
}

void Camera::renderFrame(const std::function<void()> &render)
{
    renderer_->beginCamera(this, renderTarget_.get());
    render();
    renderer_->endCamera(this, renderTarget_.get());
}

auto Camera::windowPointToWorldRay(const Vector2 &pt) const -> Ray
{
    const auto halfHeightInWorldUnits = zNear_ * std::tan(fov_.toRawRadians() / 2);
    const auto halfWidthInWorldUnits = halfHeightInWorldUnits * aspectRatio_;
    const auto canvasSize = device_->getDpiIndependentCanvasSize();
    const auto right = transform_->getWorldRight() * (halfWidthInWorldUnits * (2 * pt.x() / canvasSize.x() - 1));
    const auto down = transform_->getWorldDown() * (halfHeightInWorldUnits * (2 * pt.y() / canvasSize.y() - 1));
    const auto pos = transform_->getWorldPosition();
    const auto canvasCenter = pos + transform_->getWorldForward() * zNear_;
    const auto origin = canvasCenter + right + down;
    return Ray{origin, (origin - pos).normalized()};
}
