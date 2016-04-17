#include "SoloTransform.h"
#include "SoloCamera.h"
#include <algorithm>

using namespace solo;


const uint32_t DirtyBitPosition = 1;
const uint32_t DirtyBitRotation = 2;
const uint32_t DirtyBitScale = 4;
const uint32_t DirtyBitWorld = 8;
const uint32_t DirtyBitInverseTransposedWorld = 16;
const uint32_t DirtyBitAll =
               DirtyBitPosition |
               DirtyBitRotation |
               DirtyBitScale |
               DirtyBitWorld |
               DirtyBitInverseTransposedWorld;


Transform::Transform(Node node):
    ComponentBase(node),
    dirtyFlags(~0)
{
}


void Transform::notifyChanged() const
{
    for (auto callback : callbacks)
        callback->onTransformChanged(this);
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
    setDirtyWithChildren(DirtyBitWorld);
}


auto Transform::getParent() const -> Transform*
{
    return parent;
}


auto Transform::getChild(uint32_t index) const -> Transform*
{
    return children[index];
}


void Transform::removeChildren()
{
    while (!children.empty())
    {
        auto child = *children.begin();
        child->setParent(nullptr);
    }
}


auto Transform::getChildrenCount() const -> uint32_t
{
    return static_cast<uint32_t>(children.size());
}


auto Transform::getLocalScale() const -> Vector3
{
    return localScale;
}


auto Transform::getWorldRotation() const -> Quaternion
{
    return getWorldMatrix().getRotation();
}


auto Transform::getWorldScale() const -> Vector3
{
    return getWorldMatrix().getScale();
}


auto Transform::getLocalRotation() const -> Quaternion
{
    return getMatrix().getRotation();
}


auto Transform::getMatrix() const -> Matrix
{
    if (dirtyFlags)
    {
        auto hasTranslation = !localPosition.isZero();
        auto hasScale = !localScale.isUnit();
        auto hasRotation = !localRotation.isIdentity();

        if (hasTranslation || dirtyFlags & DirtyBitPosition)
        {
            matrix = Matrix::createTranslation(localPosition);
            if (hasRotation || dirtyFlags & DirtyBitRotation)
                matrix.rotateByQuaternion(localRotation);
            if (hasScale || dirtyFlags & DirtyBitScale)
                matrix.scaleByVector(localScale);
        }
        else if (hasRotation || dirtyFlags & DirtyBitRotation)
        {
            matrix = Matrix::createRotationFromQuaternion(localRotation);
            if (hasScale || dirtyFlags & DirtyBitScale)
                matrix.scaleByVector(localScale);
        }
        else if (hasScale || dirtyFlags & DirtyBitScale)
            matrix = Matrix::createScale(localScale);

        dirtyFlags = 0;
    }
    return matrix;
}


auto Transform::getWorldMatrix() const -> Matrix
{
    if (dirtyFlags & DirtyBitWorld)
    {
        if (parent)
            worldMatrix = parent->getWorldMatrix() * getMatrix();
        else
            worldMatrix = getMatrix();
        dirtyFlags &= ~DirtyBitWorld;
        setChildrenDirty(DirtyBitWorld);
        setDirtyWithChildren(DirtyBitInverseTransposedWorld);
    }
    return worldMatrix;
}


auto Transform::getInverseTransposedWorldMatrix() const -> Matrix
{
    if (dirtyFlags & DirtyBitInverseTransposedWorld || dirtyFlags & DirtyBitWorld)
    {
        inverseTransposedWorldMatrix = getWorldMatrix();
        inverseTransposedWorldMatrix.invert();
        inverseTransposedWorldMatrix.transpose();
        dirtyFlags &= ~DirtyBitInverseTransposedWorld;
        setChildrenDirty(DirtyBitInverseTransposedWorld);
    }
    return inverseTransposedWorldMatrix;
}


auto Transform::getWorldViewMatrix(Camera* camera) const -> Matrix
{
    return camera->getViewMatrix() * getWorldMatrix();
}


auto Transform::getWorldViewProjectionMatrix(Camera* camera) const -> Matrix
{
    return camera->getViewProjectionMatrix() * getWorldMatrix();
}


auto Transform::getInverseTransposedWorldViewMatrix(Camera* camera) const -> Matrix
{
    auto result = camera->getViewMatrix() * getWorldMatrix();
    result.invert();
    result.transpose();
    return result;
}


void Transform::translateLocal(const Vector3& translation)
{
    localPosition += translation;
    setDirtyWithChildren(DirtyBitPosition | DirtyBitWorld);
}


void Transform::rotate(const Quaternion& rotation, TransformSpace space)
{
    auto normalizedRotation(const_cast<Quaternion&>(rotation));
    normalizedRotation.normalize();

    switch (space)
    {
        case TransformSpace::Self:
            localRotation = localRotation * normalizedRotation;
            setDirtyWithChildren(DirtyBitRotation | DirtyBitWorld);
            break;
        case TransformSpace::Parent:
            localRotation = normalizedRotation * localRotation;
            setDirtyWithChildren(DirtyBitRotation | DirtyBitWorld);
            break;
        case TransformSpace::World:
        {
            auto inverseWorldRotation = getWorldRotation();
            inverseWorldRotation.inverse();
            localRotation = localRotation * inverseWorldRotation * normalizedRotation * getWorldRotation();
            setDirtyWithChildren(DirtyBitRotation | DirtyBitWorld);
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
    setDirtyWithChildren(DirtyBitScale | DirtyBitWorld);
}


void Transform::setLocalScale(const Vector3& scale)
{
    localScale = scale;
    setDirtyWithChildren(DirtyBitScale | DirtyBitWorld);
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
    setDirtyWithChildren(DirtyBitRotation | DirtyBitWorld);
}


void Transform::setLocalRotation(const Vector3& axis, const Radian& angle)
{
    localRotation = Quaternion::createFromAxisAngle(axis, angle);
    setDirtyWithChildren(DirtyBitRotation | DirtyBitWorld);
}


void Transform::setLocalPosition(const Vector3& position)
{
    localPosition = position;
    setDirtyWithChildren(DirtyBitPosition | DirtyBitWorld);
}


auto Transform::getLocalPosition() const -> Vector3
{
    return localPosition;
}


auto Transform::getWorldPosition() const -> Vector3
{
    return getWorldMatrix().getTranslation();
}


auto Transform::getLocalUp() const -> Vector3
{
    return getMatrix().getUpVector();
}


auto Transform::getLocalDown() const -> Vector3
{
    return getMatrix().getDownVector();
}


auto Transform::getLocalLeft() const -> Vector3
{
    return getMatrix().getLeftVector();
}


auto Transform::getLocalRight() const -> Vector3
{
    return getMatrix().getRightVector();
}


auto Transform::getLocalForward() const -> Vector3
{
    return getMatrix().getForwardVector();
}


auto Transform::getLocalBack() const -> Vector3
{
    return getMatrix().getBackVector();
}


void Transform::setDirtyWithChildren(uint32_t flags) const
{
    dirtyFlags |= flags;
    notifyChanged();
    for (auto child : children)
        child->setDirtyWithChildren(flags);
}


void Transform::setChildrenDirty(uint32_t flags) const
{
    for (auto child : children)
        child->setDirtyWithChildren(flags);
}
