/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloTransform.h"
#include "SoloCamera.h"
#include <algorithm>

using namespace solo;

Transform::Transform(const Node &node):
    ComponentBase(node)
{
}

void Transform::init()
{
    localScale_ = Vector3(1, 1, 1);
}

void Transform::terminate()
{
    if (this->parent_)
    {
        auto &parentChildren = this->parent_->children_;
        this->parent_->children_.erase(std::remove(parentChildren.begin(), parentChildren.end(), this), parentChildren.end());
    }
}

void Transform::setParent(Transform *parent)
{
    if (parent == this || parent == this->parent_)
        return;

    const auto worldPos = worldPosition();
    const auto worldRot = worldRotation();

    if (this->parent_)
    {
        auto &parentChildren = this->parent_->children_;
        this->parent_->children_.erase(std::remove(parentChildren.begin(), parentChildren.end(), this), parentChildren.end());
    }

    this->parent_ = parent;
    if (parent)
        parent->children_.push_back(this);

    setDirtyWithChildren();
    setWorldPosition(worldPos);
    setWorldRotation(worldRot);
}

void Transform::clearChildren()
{
    while (!children_.empty())
    {
        auto child = *children_.begin();
        child->setParent(nullptr);
    }
}

auto Transform::matrix() const -> Matrix
{
    if (dirty_)
    {
        matrix_ = Matrix::createTranslation(localPosition_);
        matrix_.rotateByQuaternion(localRotation_);
        matrix_.scaleByVector(localScale_);
        dirty_ = false;
    }
    return matrix_;
}

auto Transform::worldMatrix() const -> Matrix
{
    if (dirty_)
    {
        worldMatrix_ = parent_ ? parent_->worldMatrix() * matrix() : matrix();
        dirty_ = false;
    }
    return worldMatrix_;
}

auto Transform::invTransposedWorldMatrix() const -> Matrix
{
    if (dirty_)
    {
        invTransposedWorldMatrix_ = worldMatrix().inverted().transposed();
        dirty_ = false;
    }
    return invTransposedWorldMatrix_;
}

auto Transform::worldViewMatrix(const Camera *camera) const -> Matrix
{
    return camera->viewMatrix() * worldMatrix();
}

auto Transform::worldViewProjMatrix(const Camera *camera) const -> Matrix
{
    return camera->viewProjectionMatrix() * worldMatrix();
}

auto Transform::invTransposedWorldViewMatrix(const Camera *camera) const -> Matrix
{
    return (camera->viewMatrix() * worldMatrix()).inverted().transposed();
}

void Transform::translateLocal(const Vector3 &translation)
{
    localPosition_ += translation;
    setDirtyWithChildren();
}

void Transform::rotate(const Quaternion &rotation, TransformSpace space)
{
    const auto normalizedRotation = rotation.normalized();

    switch (space)
    {
    case TransformSpace::Self:
        localRotation_ = localRotation_ * normalizedRotation;
        break;
    case TransformSpace::Parent:
        localRotation_ = normalizedRotation * localRotation_;
        break;
    case TransformSpace::World:
        {
            const auto invWorldRotation = worldRotation().inverted();
            localRotation_ = localRotation_ * invWorldRotation * normalizedRotation * worldRotation();
            break;
        }
    default:
        break;
    }

    setDirtyWithChildren();
}

void Transform::rotateByAxisAngle(const Vector3 &axis, const Radians &angle, TransformSpace space)
{
    const auto rotation = Quaternion::fromAxisAngle(axis, angle);
    rotate(rotation, space);
}

void Transform::scaleLocal(const Vector3 &scale)
{
    localScale_.x() *= scale.x();
    localScale_.y() *= scale.y();
    localScale_.z() *= scale.z();
    setDirtyWithChildren();
}

void Transform::setWorldRotation(const Quaternion &rotation)
{
    const auto normalizedRotation = rotation.normalized();
    const auto invWorldRotation = worldRotation().inverted();
    setLocalRotation(localRotation_ * invWorldRotation * normalizedRotation);
}

void Transform::setLocalScale(const Vector3 &scale)
{
    localScale_ = scale;
    setDirtyWithChildren();
}

void Transform::lookAt(const Vector3 &target, const Vector3 &up)
{
    auto localTarget = target;
    auto localUp = up;

    if (parent_)
    {
        const auto m = parent_->worldMatrix().inverted();
        localTarget = m.transformPoint(target);
        localUp = m.transformDirection(up);
    }

    const auto lookAtMatrix = Matrix::createLookAt(localPosition_, localTarget, localUp);
    setLocalRotation(lookAtMatrix.rotation());
}

auto Transform::transformPoint(const Vector3 &point) const -> Vector3
{
    return matrix().transformPoint(point);
}

auto Transform::transformDirection(const Vector3 &direction) const -> Vector3
{
    return matrix().transformDirection(direction);
}

void Transform::setLocalRotation(const Quaternion &rotation)
{
    localRotation_ = rotation;
    setDirtyWithChildren();
}

void Transform::setLocalAxisAngleRotation(const Vector3 &axis, const Radians &angle)
{
    localRotation_ = Quaternion::fromAxisAngle(axis, angle);
    setDirtyWithChildren();
}

void Transform::setWorldPosition(const Vector3 &position)
{
    auto localPos = position;
    if (parent_)
    {
        const auto worldMat = parent_->worldMatrix().inverted();
        localPos = worldMat.transformPoint(position);
    }
    setLocalPosition(localPos);
}

void Transform::setLocalPosition(const Vector3 &position)
{
    localPosition_ = position;
    setDirtyWithChildren();
}

void Transform::setDirtyWithChildren() const
{
    dirty_ = true;
    version_++;
    for (auto child : children_)
        child->setDirtyWithChildren();
}

void Transform::setChildrenDirty() const
{
    for (auto child : children_)
        child->setDirtyWithChildren();
}
