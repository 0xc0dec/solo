#include "SoloTransform.h"
#include "SoloCamera.h"
#include <algorithm>

using namespace solo;


Transform::Transform(const Node& node):
    ComponentBase(node),
    dirtyFlags(~0)
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


void Transform::addCallback(TransformCallback* callback)
{
    callbacks.push_back(callback);
}


void Transform::removeCallback(TransformCallback* callback)
{
    callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
}


void Transform::setParent(Transform* parent)
{
    if (parent == this || parent == this->parent)
        return;
    if (this->parent)
    {
        auto& parentChildren = this->parent->children;
        this->parent->children.erase(std::remove(parentChildren.begin(), parentChildren.end(), this), parentChildren.end());
    }
    this->parent = parent;
    if (parent)
        parent->children.push_back(this);
    setDirtyWithChildren(TransformDirtyFlags::World);
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
    if (dirtyFlags)
    {
        auto hasTranslation = !localPosition.isZero();
        auto hasScale = !localScale.isUnit();
        auto hasRotation = !localRotation.isIdentity();

        if (hasTranslation || dirtyFlags & TransformDirtyFlags::Position)
        {
            matrix = Matrix::createTranslation(localPosition);
            if (hasRotation || dirtyFlags & TransformDirtyFlags::Rotation)
                matrix.rotateByQuaternion(localRotation);
            if (hasScale || dirtyFlags & TransformDirtyFlags::Scale)
                matrix.scaleByVector(localScale);
        }
        else if (hasRotation || dirtyFlags & TransformDirtyFlags::Rotation)
        {
            matrix = Matrix::createRotationFromQuaternion(localRotation);
            if (hasScale || dirtyFlags & TransformDirtyFlags::Scale)
                matrix.scaleByVector(localScale);
        }
        else if (hasScale || dirtyFlags & TransformDirtyFlags::Scale)
            matrix = Matrix::createScale(localScale);

        dirtyFlags = 0;
    }
    return matrix;
}


auto Transform::getWorldMatrix() const -> Matrix
{
    if (dirtyFlags & TransformDirtyFlags::World)
    {
        if (parent)
            worldMatrix = parent->getWorldMatrix() * getMatrix();
        else
            worldMatrix = getMatrix();
        dirtyFlags &= ~TransformDirtyFlags::World;
        setChildrenDirty(TransformDirtyFlags::World);
        setDirtyWithChildren(TransformDirtyFlags::InvTransposedWorld);
    }
    return worldMatrix;
}


auto Transform::getInvTransposedWorldMatrix() const -> Matrix
{
    if (dirtyFlags & TransformDirtyFlags::InvTransposedWorld || dirtyFlags & TransformDirtyFlags::World)
    {
        invTransposedWorldMatrix = getWorldMatrix();
        invTransposedWorldMatrix.invert();
        invTransposedWorldMatrix.transpose();
        dirtyFlags &= ~TransformDirtyFlags::InvTransposedWorld;
        setChildrenDirty(TransformDirtyFlags::InvTransposedWorld);
    }
    return invTransposedWorldMatrix;
}


auto Transform::getWorldViewMatrix(Camera* camera) const -> Matrix
{
    return camera->getViewMatrix() * getWorldMatrix();
}


auto Transform::getWorldViewProjectionMatrix(Camera* camera) const -> Matrix
{
    return camera->getViewProjectionMatrix() * getWorldMatrix();
}


auto Transform::getInvTransposedWorldViewMatrix(Camera* camera) const -> Matrix
{
    auto result = camera->getViewMatrix() * getWorldMatrix();
    result.invert();
    result.transpose();
    return result;
}


void Transform::translateLocal(const Vector3& translation)
{
    localPosition += translation;
    setDirtyWithChildren(TransformDirtyFlags::Position | TransformDirtyFlags::World);
}


void Transform::rotate(const Quaternion& rotation, TransformSpace space)
{
    auto normalizedRotation(const_cast<Quaternion&>(rotation));
    normalizedRotation.normalize();

    switch (space)
    {
        case TransformSpace::Self:
            localRotation = localRotation * normalizedRotation;
            setDirtyWithChildren(TransformDirtyFlags::Rotation | TransformDirtyFlags::World);
            break;
        case TransformSpace::Parent:
            localRotation = normalizedRotation * localRotation;
            setDirtyWithChildren(TransformDirtyFlags::Rotation | TransformDirtyFlags::World);
            break;
        case TransformSpace::World:
        {
            auto invWorldRotation = getWorldRotation();
            invWorldRotation.inverse();
            localRotation = localRotation * invWorldRotation * normalizedRotation * getWorldRotation();
            setDirtyWithChildren(TransformDirtyFlags::Rotation | TransformDirtyFlags::World);
            break;
        }
    }
}


void Transform::rotate(const Vector3& axis, const Radian& angle, TransformSpace space)
{
    auto rotation = Quaternion::createFromAxisAngle(axis, angle);
    rotate(rotation, space);
}


void Transform::scaleLocal(const Vector3& scale)
{
    localScale.x *= scale.x;
    localScale.y *= scale.y;
    localScale.z *= scale.z;
    setDirtyWithChildren(TransformDirtyFlags::Scale | TransformDirtyFlags::World);
}


void Transform::setLocalScale(const Vector3& scale)
{
    localScale = scale;
    setDirtyWithChildren(TransformDirtyFlags::Scale | TransformDirtyFlags::World);
}


void Transform::lookAt(const Vector3& target, const Vector3& up)
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


auto Transform::transformPoint(const Vector3& point) const -> Vector3
{
    return getMatrix().transformPoint(point);
}


auto Transform::transformDirection(const Vector3& direction) const -> Vector3
{
    return getMatrix().transformDirection(direction);
}


void Transform::setLocalRotation(const Quaternion& rotation)
{
    localRotation = rotation;
    setDirtyWithChildren(TransformDirtyFlags::Rotation | TransformDirtyFlags::World);
}


void Transform::setLocalRotation(const Vector3& axis, const Radian& angle)
{
    localRotation = Quaternion::createFromAxisAngle(axis, angle);
    setDirtyWithChildren(TransformDirtyFlags::Rotation | TransformDirtyFlags::World);
}


void Transform::setLocalPosition(const Vector3& position)
{
    localPosition = position;
    setDirtyWithChildren(TransformDirtyFlags::Position | TransformDirtyFlags::World);
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
