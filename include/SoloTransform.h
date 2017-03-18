/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloComponent.h"
#include "SoloTransformMatrix.h"
#include "SoloVector3.h"
#include "SoloQuaternion.h"
#include "SoloNode.h"
#include <vector>

namespace solo
{
    class Camera;
    class TransformCallback;
    class Transform;
    struct Radian;

    enum class TransformSpace
    {
        Self,
        Parent,
        World
    };

    // NB Using struct with consts instead of class enums
    // due to easier work with them as bit flags
    struct TransformDirtyFlags final
    {
        static const uint32_t Position = 1 << 0;
        static const uint32_t Rotation = 1 << 1;
        static const uint32_t Scale = 1 << 2;
        static const uint32_t World = 1 << 3;
        static const uint32_t InvTransposedWorld = 1 << 4;
    };

    class TransformCallback
    {
    public:
        virtual ~TransformCallback() {}

        virtual void onTransformChanged(const Transform *transform, uint32_t dirtyFlags) = 0;
    };

    class Transform final: public ComponentBase<Transform>
    {
    public:
        explicit Transform(const Node &node);

        void init() override final;

        void addCallback(TransformCallback *callback);
        void removeCallback(TransformCallback *callback);

        void setParent(Transform *parent);
        auto getParent() const -> Transform*;
        
        auto getChild(uint32_t index) const -> Transform*;
        auto getChildrenCount() const -> uint32_t;
        void clearChildren();

        auto getWorldScale() const -> Vector3;
        auto getLocalScale() const -> Vector3;

        auto getWorldRotation() const -> Quaternion;
        auto getLocalRotation() const -> Quaternion;

        auto getWorldPosition() const -> Vector3;
        auto getLocalPosition() const -> Vector3;

        auto getWorldUp() const -> Vector3;
        auto getLocalUp() const -> Vector3;

        auto getWorldDown() const -> Vector3;
        auto getLocalDown() const -> Vector3;

        auto getWorldLeft() const -> Vector3;
        auto getLocalLeft() const -> Vector3;

        auto getWorldRight() const -> Vector3;
        auto getLocalRight() const -> Vector3;

        auto getWorldForward() const -> Vector3;
        auto getLocalForward() const -> Vector3;

        auto getWorldBack() const -> Vector3;
        auto getLocalBack() const -> Vector3;

        void translateLocal(const Vector3 &translation);
        void scaleLocal(const Vector3 &scale);

        void setLocalPosition(const Vector3 &position);
        void setLocalScale(const Vector3 &scale);

        void rotate(const Quaternion &rotation, TransformSpace space = TransformSpace::Self);
        void rotateByAxisAngle(const Vector3 &axis, const Radian &angle, TransformSpace space = TransformSpace::Self);

        void setLocalRotation(const Quaternion &rotation);
        void setLocalAxisAngleRotation(const Vector3 &axis, const Radian &angle);

        void lookAt(const Vector3 &target, const Vector3 &up);

        auto getMatrix() const -> TransformMatrix;
        auto getWorldMatrix() const -> TransformMatrix;

        auto getWorldViewMatrix(const Camera *camera) const -> TransformMatrix;
        auto getWorldViewProjMatrix(const Camera *camera) const -> TransformMatrix;
        auto getInvTransposedWorldViewMatrix(const Camera *camera) const -> TransformMatrix;
        auto getInvTransposedWorldMatrix() const -> TransformMatrix;

        auto transformPoint(const Vector3 &point) const -> Vector3;
        auto transformDirection(const Vector3 &direction) const -> Vector3;

    private:
        mutable uint32_t dirtyFlags = ~0;

        Transform *parent = nullptr;
        std::vector<Transform *> children;
        std::vector<TransformCallback *> callbacks;

        Vector3 localPosition;
        Vector3 localScale;
        Quaternion localRotation;
        mutable TransformMatrix matrix;
        mutable TransformMatrix worldMatrix;
        mutable TransformMatrix invTransposedWorldMatrix;

        void setDirtyWithChildren(uint32_t flags) const;
        void setChildrenDirty(uint32_t flags) const;
        void notifyChanged(uint32_t dirtyFlags) const;
    };

    inline auto Transform::getLocalPosition() const -> Vector3
    {
        return localPosition;
    }

    inline auto Transform::getWorldPosition() const -> Vector3
    {
        return getWorldMatrix().getTranslation();
    }

    inline auto Transform::getWorldUp() const -> Vector3
    {
        return getWorldMatrix().getUpVector();
    }

    inline auto Transform::getLocalUp() const -> Vector3
    {
        return getMatrix().getUpVector();
    }

    inline auto Transform::getWorldDown() const -> Vector3
    {
        return getWorldMatrix().getDownVector();
    }

    inline auto Transform::getLocalDown() const -> Vector3
    {
        return getMatrix().getDownVector();
    }

    inline auto Transform::getWorldLeft() const -> Vector3
    {
        return getWorldMatrix().getLeftVector();
    }

    inline auto Transform::getLocalLeft() const -> Vector3
    {
        return getMatrix().getLeftVector();
    }

    inline auto Transform::getWorldRight() const -> Vector3
    {
        return getWorldMatrix().getRightVector();
    }

    inline auto Transform::getLocalRight() const -> Vector3
    {
        return getMatrix().getRightVector();
    }

    inline auto Transform::getWorldForward() const -> Vector3
    {
        return getWorldMatrix().getForwardVector();
    }

    inline auto Transform::getLocalForward() const -> Vector3
    {
        return getMatrix().getForwardVector();
    }

    inline auto Transform::getWorldBack() const -> Vector3
    {
        return getWorldMatrix().getBackVector();
    }

    inline auto Transform::getLocalBack() const -> Vector3
    {
        return getMatrix().getBackVector();
    }

    inline auto Transform::getChildrenCount() const -> uint32_t
    {
        return static_cast<uint32_t>(children.size());
    }

    inline auto Transform::getLocalScale() const -> Vector3
    {
        return localScale;
    }

    inline auto Transform::getWorldRotation() const -> Quaternion
    {
        return getWorldMatrix().getRotation();
    }

    inline auto Transform::getWorldScale() const -> Vector3
    {
        return getWorldMatrix().getScale();
    }

    inline auto Transform::getLocalRotation() const -> Quaternion
    {
        return localRotation;
    }

    inline auto Transform::getParent() const -> Transform *
    {
        return parent;
    }

    inline auto Transform::getChild(uint32_t index) const -> Transform *
    {
        return children[index];
    }
}
