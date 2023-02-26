/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloCamera.h"
#include "SoloNode.h"
#include "SoloDevice.h"
#include "math/SoloRadians.h"
#include "math/SoloDegrees.h"
#include "SoloScene.h"
#include "math/SoloRay.h"
#include "SoloRenderer.h"

using namespace solo;

static constexpr u32 DIRTY_BIT_VIEW = 1;
static constexpr u32 DIRTY_BIT_PROJECTION = 1 << 1;
static constexpr u32 DIRTY_BIT_VIEW_PROJECTION = 1 << 2;
static constexpr u32 DIRTY_BIT_INV_VIEW = 1 << 3;
static constexpr u32 DIRTY_BIT_INV_VIEW_PROJECTION = 1 << 4;
static constexpr u32 DIRTY_BIT_ALL_PROJECTION = DIRTY_BIT_PROJECTION | DIRTY_BIT_VIEW_PROJECTION | DIRTY_BIT_INV_VIEW_PROJECTION;

auto Camera::create(const Node &node) -> sptr<Camera> {
    return std::shared_ptr<Camera>(new Camera(node));
}

Camera::Camera(const Node &node):
    ComponentBase(node),
    device_(node.scene()->device()),
    renderer_(device_->renderer()),
    fov_(Degrees(60)) {
    auto canvasSize = device_->canvasSize();
    viewport_ = Vector4(0, 0, canvasSize.x(), canvasSize.y());
}

void Camera::init() {
    transform_ = node_.findComponent<Transform>();
    const auto canvasSize = device_->canvasSize();
    aspectRatio_ = canvasSize.x() / canvasSize.y();
    dirtyFlags_ |= DIRTY_BIT_ALL_PROJECTION;
}

void Camera::update() {
    if (lastTransformVersion_ != transform_->version()) {
        lastTransformVersion_ = transform_->version();
        dirtyFlags_ |= DIRTY_BIT_VIEW | DIRTY_BIT_VIEW_PROJECTION | DIRTY_BIT_INV_VIEW | DIRTY_BIT_INV_VIEW_PROJECTION;
    }
}

void Camera::setPerspective(bool perspective) {
    ortho_ = !perspective;
    dirtyFlags_ |= DIRTY_BIT_ALL_PROJECTION;
}

void Camera::setFieldOfView(const Radians &fov) {
    this->fov_ = fov;
    dirtyFlags_ |= DIRTY_BIT_ALL_PROJECTION;
}

void Camera::setOrthoSize(const Vector2 &size) {
    orthoSize_ = size;
    dirtyFlags_ |= DIRTY_BIT_ALL_PROJECTION;
}

void Camera::setZFar(float far) {
    this->zFar_ = far;
    dirtyFlags_ |= DIRTY_BIT_ALL_PROJECTION;
}

void Camera::setZNear(float near) {
    this->zNear_ = near;
    dirtyFlags_ |= DIRTY_BIT_ALL_PROJECTION;
}

auto Camera::viewMatrix() const -> Matrix {
    if (dirtyFlags_ & DIRTY_BIT_VIEW) {
        viewMatrix_ = transform_->worldMatrix().inverted();
        dirtyFlags_ &= ~DIRTY_BIT_VIEW;
    }
    return viewMatrix_;
}

auto Camera::invViewMatrix() const -> Matrix {
    if (dirtyFlags_ & DIRTY_BIT_INV_VIEW) {
        invViewMatrix_ = viewMatrix().inverted();
        dirtyFlags_ &= ~DIRTY_BIT_INV_VIEW;
    }
    return invViewMatrix_;
}

auto Camera::projectionMatrix() const -> Matrix {
    if (dirtyFlags_ & DIRTY_BIT_PROJECTION) {
        if (ortho_)
            projectionMatrix_ = Matrix::createOrthographic(orthoSize_.x(), orthoSize_.y(), zNear_, zFar_);
        else
            projectionMatrix_ = Matrix::createPerspective(fov_, aspectRatio_, zNear_, zFar_);
        dirtyFlags_ &= ~DIRTY_BIT_PROJECTION;
    }
    return projectionMatrix_;
}

auto Camera::viewProjectionMatrix() const -> Matrix {
    if (dirtyFlags_ & DIRTY_BIT_VIEW_PROJECTION) {
        viewProjectionMatrix_ = projectionMatrix() * viewMatrix();
        dirtyFlags_ &= ~DIRTY_BIT_VIEW_PROJECTION;
    }
    return viewProjectionMatrix_;
}

auto Camera::invViewProjectionMatrix() const -> Matrix {
    if (dirtyFlags_ & DIRTY_BIT_INV_VIEW_PROJECTION) {
        invViewProjectionMatrix_ = viewProjectionMatrix().inverted();
        dirtyFlags_ &= ~DIRTY_BIT_INV_VIEW_PROJECTION;
    }
    return invViewProjectionMatrix_;
}

void Camera::renderFrame(const std::function<void()> &render) {
    renderer_->beginCamera(this);
    render();
    renderer_->endCamera(this);
}

auto Camera::windowPointToWorldRay(const Vector2 &pt) const -> Ray {
    const auto halfHeightInWorldUnits = zNear_ * std::tan(fov_.toRawRadians() / 2);
    const auto halfWidthInWorldUnits = halfHeightInWorldUnits * aspectRatio_;
    const auto canvasSize = device_->dpiIndependentCanvasSize();
    const auto right = transform_->worldRight() * (halfWidthInWorldUnits * (2 * pt.x() / canvasSize.x() - 1));
    const auto down = transform_->worldDown() * (halfHeightInWorldUnits * (2 * pt.y() / canvasSize.y() - 1));
    const auto pos = transform_->worldPosition();
    const auto canvasCenter = pos + transform_->worldForward() * zNear_;
    const auto origin = canvasCenter + right + down;
    return Ray{origin, (origin - pos).normalized()};
}
