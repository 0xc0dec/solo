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

#include "SoloTransform.h"
#include "SoloCamera.h"
#include <algorithm>

using namespace solo;


Transform::Transform(const Node &node):
    ComponentBase(node)
{
}


void Transform::notifyChanged(uint32_t dirtyFlags) const
{
    for (auto callback : callbacks)
        callback->onTransformChanged(this, dirtyFlags);
}


void Transform::init()
{
    localScale = Vector3::unit();
}


void Transform::addCallback(TransformCallback *callback)
{
    callbacks.push_back(callback);
}


void Transform::removeCallback(TransformCallback *callback)
{
    callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
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
    setDirtyWithChildren(TransformDirtyFlags::World | TransformDirtyFlags::InvTransposedWorld);
}


void Transform::clearChildren()
{
    while (!children.empty())
    {
        auto child = *children.begin();
        child->setParent(nullptr);
    }
}


auto Transform::getMatrix() const -> TransformMatrix
{
    auto dirty = dirtyFlags & TransformDirtyFlags::Position ||
                 dirtyFlags & TransformDirtyFlags::Rotation ||
                 dirtyFlags & TransformDirtyFlags::Scale;
    if (dirty)
    {
        if (dirtyFlags & TransformDirtyFlags::Position || !localPosition.isZero())
        {
            matrix = TransformMatrix::createTranslation(localPosition);
            if (dirtyFlags & TransformDirtyFlags::Rotation || !localRotation.isIdentity())
                matrix.rotateByQuaternion(localRotation);
            if (dirtyFlags & TransformDirtyFlags::Scale || !localScale.isUnit())
                matrix.scaleByVector(localScale);
        }
        else if (dirtyFlags & TransformDirtyFlags::Rotation || !localRotation.isIdentity())
        {
            matrix = TransformMatrix::createRotationFromQuaternion(localRotation);
            if (dirtyFlags & TransformDirtyFlags::Scale || !localScale.isUnit())
                matrix.scaleByVector(localScale);
        }
        else if (dirtyFlags & TransformDirtyFlags::Scale || !localScale.isUnit())
            matrix = TransformMatrix::createScale(localScale);

        dirtyFlags &= ~(TransformDirtyFlags::Position | TransformDirtyFlags::Rotation | TransformDirtyFlags::Scale);
    }
    return matrix;
}


auto Transform::getWorldMatrix() const -> TransformMatrix
{
    if (dirtyFlags & TransformDirtyFlags::World)
    {
        if (parent)
            worldMatrix = parent->getWorldMatrix() * getMatrix();
        else
            worldMatrix = getMatrix();
        dirtyFlags &= ~TransformDirtyFlags::World;
    }
    return worldMatrix;
}


auto Transform::getInvTransposedWorldMatrix() const -> TransformMatrix
{
    if (dirtyFlags & TransformDirtyFlags::InvTransposedWorld)
    {
        invTransposedWorldMatrix = getWorldMatrix();
        invTransposedWorldMatrix.invert();
        invTransposedWorldMatrix.transpose();
        dirtyFlags &= ~TransformDirtyFlags::InvTransposedWorld;
    }
    return invTransposedWorldMatrix;
}


auto Transform::getWorldViewMatrix(const Camera *camera) const -> TransformMatrix
{
    return camera->getViewMatrix() * getWorldMatrix();
}


auto Transform::getWorldViewProjectionMatrix(const Camera *camera) const -> TransformMatrix
{
    return camera->getViewProjectionMatrix() * getWorldMatrix();
}


auto Transform::getInvTransposedWorldViewMatrix(const Camera *camera) const -> TransformMatrix
{
    auto result = camera->getViewMatrix() * getWorldMatrix();
    result.invert();
    result.transpose();
    return result;
}


void Transform::translateLocal(const Vector3 &translation)
{
    localPosition += translation;
    setDirtyWithChildren(TransformDirtyFlags::Position | TransformDirtyFlags::World | TransformDirtyFlags::InvTransposedWorld);
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
            invWorldRotation.inverse();
            localRotation = localRotation * invWorldRotation * normalizedRotation * getWorldRotation();
            break;
        }
        default:
            break;
    }

    setDirtyWithChildren(TransformDirtyFlags::Rotation | TransformDirtyFlags::World | TransformDirtyFlags::InvTransposedWorld);
}


void Transform::rotate(const Vector3 &axis, const Radian &angle, TransformSpace space)
{
    auto rotation = Quaternion::createFromAxisAngle(axis, angle);
    rotate(rotation, space);
}


void Transform::scaleLocal(const Vector3 &scale)
{
    localScale.x *= scale.x;
    localScale.y *= scale.y;
    localScale.z *= scale.z;
    setDirtyWithChildren(TransformDirtyFlags::Scale | TransformDirtyFlags::World | TransformDirtyFlags::InvTransposedWorld);
}


void Transform::setLocalScale(const Vector3 &scale)
{
    localScale = scale;
    setDirtyWithChildren(TransformDirtyFlags::Scale | TransformDirtyFlags::World | TransformDirtyFlags::InvTransposedWorld);
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

    auto lookAtMatrix = TransformMatrix::createLookAt(localPosition, localTarget, localUp);
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
    setDirtyWithChildren(TransformDirtyFlags::Rotation | TransformDirtyFlags::World | TransformDirtyFlags::InvTransposedWorld);
}


void Transform::setLocalRotation(const Vector3 &axis, const Radian &angle)
{
    localRotation = Quaternion::createFromAxisAngle(axis, angle);
    setDirtyWithChildren(TransformDirtyFlags::Rotation | TransformDirtyFlags::World | TransformDirtyFlags::InvTransposedWorld);
}


void Transform::setLocalPosition(const Vector3 &position)
{
    localPosition = position;
    setDirtyWithChildren(TransformDirtyFlags::Position | TransformDirtyFlags::World | TransformDirtyFlags::InvTransposedWorld);
}


void Transform::setDirtyWithChildren(uint32_t flags) const
{
    dirtyFlags |= flags;
    notifyChanged(flags);
    for (auto child : children)
        child->setDirtyWithChildren(flags);
}


void Transform::setChildrenDirty(uint32_t flags) const
{
    for (auto child : children)
        child->setDirtyWithChildren(flags);
}
