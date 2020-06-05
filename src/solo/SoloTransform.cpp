/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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

	setDirtyWithChildren(DirtyFlagAll);
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
    if (dirtyFlags_ & DirtyFlagLocal)
    {
        matrix_ = Matrix::createTranslation(localPosition_);
        matrix_.rotateByQuaternion(localRotation_);
        matrix_.scaleByVector(localScale_);
        dirtyFlags_ &= ~DirtyFlagLocal;
    }
    return matrix_;
}

auto Transform::worldMatrix() const -> Matrix
{
    if (dirtyFlags_ & DirtyFlagWorld)
    {
        worldMatrix_ = parent_ ? parent_->worldMatrix() * matrix() : matrix();
        dirtyFlags_ &= ~DirtyFlagWorld;
    }
    return worldMatrix_;
}

auto Transform::invTransposedWorldMatrix() const -> Matrix
{
    if (dirtyFlags_ & DirtyFlagInvTransposedWorld)
    {
        invTransposedWorldMatrix_ = worldMatrix();
        invTransposedWorldMatrix_.invert();
        invTransposedWorldMatrix_.transpose();
        dirtyFlags_ &= ~DirtyFlagInvTransposedWorld;
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
    auto result = camera->viewMatrix() * worldMatrix();
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
            auto invWorldRotation = worldRotation();
            invWorldRotation.invert();
            localRotation_ = localRotation_ * invWorldRotation * normalizedRotation * worldRotation();
            break;
        }
        default:
            break;
    }

    setDirtyWithChildren(DirtyFlagAll);
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
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::setWorldRotation(const Quaternion &rotation)
{
	auto normalizedRotation(const_cast<Quaternion &>(rotation));
    normalizedRotation.normalize();
	auto invWorldRotation = worldRotation();
    invWorldRotation.invert();
	setLocalRotation(localRotation_ * invWorldRotation * normalizedRotation);
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
        auto m(parent_->worldMatrix());
        m.invert();
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
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::setLocalAxisAngleRotation(const Vector3 &axis, const Radians &angle)
{
    localRotation_ = Quaternion::fromAxisAngle(axis, angle);
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::setWorldPosition(const Vector3 &position)
{
	auto localPos = position;
	if (parent_)
	{
		auto worldMat = parent_->worldMatrix();
		worldMat.invert();
		localPos = worldMat.transformPoint(position);
	}
	setLocalPosition(localPos);
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
