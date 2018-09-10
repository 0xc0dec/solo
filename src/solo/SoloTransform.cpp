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
    localScale = Vector3(1, 1, 1);
}

void Transform::terminate()
{
    if (this->parent)
    {
        auto &parentChildren = this->parent->children;
        this->parent->children.erase(std::remove(parentChildren.begin(), parentChildren.end(), this), parentChildren.end());
    }
}

void Transform::setParent(Transform *parent)
{
    if (parent == this || parent == this->parent)
        return;

    if (this->parent)
    {
        auto &parentChildren = this->parent->children;
        this->parent->children.erase(std::remove(parentChildren.begin(), parentChildren.end(), this), parentChildren.end());
    }

    this->parent = parent;
    if (parent)
        parent->children.push_back(this);

    setDirtyWithChildren(DirtyFlagWorld | DirtyFlagInvTransposedWorld);
}

void Transform::clearChildren()
{
    while (!children.empty())
    {
        auto child = *children.begin();
        child->setParent(nullptr);
    }
}

auto Transform::getMatrix() const -> Matrix
{
    if (dirtyFlags & DirtyFlagLocal)
    {
        matrix = Matrix::createTranslation(localPosition);
        matrix.rotateByQuaternion(localRotation);
        matrix.scaleByVector(localScale);
        dirtyFlags &= ~DirtyFlagLocal;
    }
    return matrix;
}

auto Transform::getWorldMatrix() const -> Matrix
{
    if (dirtyFlags & DirtyFlagWorld)
    {
        if (parent)
            worldMatrix = parent->getWorldMatrix() * getMatrix();
        else
            worldMatrix = getMatrix();
        dirtyFlags &= ~DirtyFlagWorld;
    }
    return worldMatrix;
}

auto Transform::getInvTransposedWorldMatrix() const -> Matrix
{
    if (dirtyFlags & DirtyFlagInvTransposedWorld)
    {
        invTransposedWorldMatrix = getWorldMatrix();
        invTransposedWorldMatrix.invert();
        invTransposedWorldMatrix.transpose();
        dirtyFlags &= ~DirtyFlagInvTransposedWorld;
    }
    return invTransposedWorldMatrix;
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
    localPosition += translation;
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::rotate(const Quaternion &rotation, TransformSpace space)
{
    auto normalizedRotation(const_cast<Quaternion &>(rotation));
    normalizedRotation.normalize();

    switch (space)
    {
        case TransformSpace::Self:
            localRotation = localRotation * normalizedRotation;
            break;
        case TransformSpace::Parent:
            localRotation = normalizedRotation * localRotation;
            break;
        case TransformSpace::World:
        {
            auto invWorldRotation = getWorldRotation();
            invWorldRotation.invert();
            localRotation = localRotation * invWorldRotation * normalizedRotation * getWorldRotation();
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
    localScale.x() *= scale.x();
    localScale.y() *= scale.y();
    localScale.z() *= scale.z();
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::setLocalScale(const Vector3 &scale)
{
    localScale = scale;
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::lookAt(const Vector3 &target, const Vector3 &up)
{
    auto localTarget = target;
    auto localUp = up;

    if (parent)
    {
        auto m(parent->getWorldMatrix());
        m.invert();
        localTarget = m.transformPoint(target);
        localUp = m.transformDirection(up);
    }

    auto lookAtMatrix = Matrix::createLookAt(localPosition, localTarget, localUp);
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
    localRotation = rotation;
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::setLocalAxisAngleRotation(const Vector3 &axis, const Radians &angle)
{
    localRotation = Quaternion::createFromAxisAngle(axis, angle);
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::setLocalPosition(const Vector3 &position)
{
    localPosition = position;
    setDirtyWithChildren(DirtyFlagAll);
}

void Transform::setDirtyWithChildren(u32 flags) const
{
    dirtyFlags |= flags;
    version++;
    for (auto child : children)
        child->setDirtyWithChildren(flags);
}

void Transform::setChildrenDirty(u32 flags) const
{
    for (auto child : children)
        child->setDirtyWithChildren(flags);
}
