/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTransform.h"
#include "SoloCamera.h"
#include <algorithm>

using namespace solo;

static const u32 DirtyFlagLocal = 1 << 0;
static const u32 DirtyFlagWorld = 1 << 1;
static const u32 DirtyFlagInvTransposedWorld = 1 << 2;
static const u32 DirtyFlagAll = DirtyFlagLocal | DirtyFlagWorld | DirtyFlagInvTransposedWorld;

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

    if (this->parent_)
    {
        auto &parentChildren = this->parent_->children_;
        this->parent_->children_.erase(std::remove(parentChildren.begin(), parentChildren.end(), this), parentChildren.end());
    }

    this->parent_ = parent;
    if (parent)
        parent->children_.push_back(this);

    setDirtyWithChildren(DirtyFlagWorld | DirtyFlagInvTransposedWorld);
}

void Transform::clearChildren()
{
    while (!children_.empty())
    {
        auto child = *children_.begin();
        child->setParent(nullptr);
    }
}

auto Transform::getMatrix() const -> Matrix
{
    if (dirtyFlags_ & DirtyFlagLocal)
    {
        matrix_ = Matrix::createTranslation(localPosition_);
        matrix_.rotateByQuaternion(localRotation_);
        matrix_.scaleByVector(localScale_);
        dirtyFlags_ &= ~DirtyFlagLocal;
    }
    return matrix_;
}

auto Transform::getWorldMatrix() const -> Matrix
{
    if (dirtyFlags_ & DirtyFlagWorld)
    {
        if (parent_)
            worldMatrix_ = parent_->getWorldMatrix() * getMatrix();
        else
            worldMatrix_ = getMatrix();
        dirtyFlags_ &= ~DirtyFlagWorld;
    }
    return worldMatrix_;
}

auto Transform::getInvTransposedWorldMatrix() const -> Matrix
{
    if (dirtyFlags_ & DirtyFlagInvTransposedWorld)
    {
        invTransposedWorldMatrix_ = getWorldMatrix();
        invTransposedWorldMatrix_.invert();
        invTransposedWorldMatrix_.transpose();
        dirtyFlags_ &= ~DirtyFlagInvTransposedWorld;
    }
    return invTransposedWorldMatrix_;
}

auto Transform::getWorldViewMatrix(const Camera *camera) const -> Matrix
{
    return camera->getViewMatrix() * getWorldMatrix();
}

auto Transform::getWorldViewProjMatrix(const Camera *camera) const -> Matrix
{
    return camera->getViewProjectionMatrix() * getWorldMatrix();
}

auto Transform::getInvTransposedWorldViewMatrix(const Camera *camera) const -> Matrix
{
    auto result = camera->getViewMatrix() * getWorldMatrix();
    result.invert();
    result.transpose();
    return result;
}

void Transform::translateLocal(const Vector3 &translation)
{
    localPosition_ += translation;
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::rotate(const Quaternion &rotation, TransformSpace space)
{
    auto normalizedRotation(const_cast<Quaternion &>(rotation));
    normalizedRotation.normalize();

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
            auto invWorldRotation = getWorldRotation();
            invWorldRotation.invert();
            localRotation_ = localRotation_ * invWorldRotation * normalizedRotation * getWorldRotation();
            break;
        }
        default:
            break;
    }

    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::rotateByAxisAngle(const Vector3 &axis, const Radians &angle, TransformSpace space)
{
    const auto rotation = Quaternion::createFromAxisAngle(axis, angle);
    rotate(rotation, space);
}

void Transform::scaleLocal(const Vector3 &scale)
{
    localScale_.x() *= scale.x();
    localScale_.y() *= scale.y();
    localScale_.z() *= scale.z();
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::setLocalScale(const Vector3 &scale)
{
    localScale_ = scale;
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::lookAt(const Vector3 &target, const Vector3 &up)
{
    auto localTarget = target;
    auto localUp = up;

    if (parent_)
    {
        auto m(parent_->getWorldMatrix());
        m.invert();
        localTarget = m.transformPoint(target);
        localUp = m.transformDirection(up);
    }

    auto lookAtMatrix = Matrix::createLookAt(localPosition_, localTarget, localUp);
    setLocalRotation(lookAtMatrix.getRotation());
}

auto Transform::transformPoint(const Vector3 &point) const -> Vector3
{
    return getMatrix().transformPoint(point);
}

auto Transform::transformDirection(const Vector3 &direction) const -> Vector3
{
    return getMatrix().transformDirection(direction);
}

void Transform::setLocalRotation(const Quaternion &rotation)
{
    localRotation_ = rotation;
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::setLocalAxisAngleRotation(const Vector3 &axis, const Radians &angle)
{
    localRotation_ = Quaternion::createFromAxisAngle(axis, angle);
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::setLocalPosition(const Vector3 &position)
{
    localPosition_ = position;
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::setDirtyWithChildren(u32 flags) const
{
    dirtyFlags_ |= flags;
    version_++;
    for (auto child : children_)
        child->setDirtyWithChildren(flags);
}

void Transform::setChildrenDirty(u32 flags) const
{
    for (auto child : children_)
        child->setDirtyWithChildren(flags);
}
