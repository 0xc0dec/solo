/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloComponent.h"
#include "SoloVector3.h"
#include "SoloQuaternion.h"
#include "SoloMatrix.h"
#include "SoloNode.h"

namespace solo
{
    class Camera;
    class Transform;
    struct Radians;

    enum class TransformSpace
    {
        Self,
        Parent,
        World
    };

    class Transform final: public ComponentBase<Transform>
    {
    public:
        explicit Transform(const Node &node);

        void init() override final;
        void terminate() override final;

        auto version() const -> u32 { return version_; }

        auto parent() const -> Transform* { return parent_; }
        void setParent(Transform *parent);
        
        auto child(u32 index) const -> Transform* { return children_[index]; }
        auto childrenCount() const -> u32 { return static_cast<u32>(children_.size()); }
        void clearChildren();

        auto worldScale() const -> Vector3 { return worldMatrix().scale(); }
        auto localScale() const -> Vector3 { return localScale_; }

        auto worldRotation() const -> Quaternion { return worldMatrix().rotation(); }
        auto localRotation() const -> Quaternion { return localRotation_; }

        auto worldPosition() const -> Vector3 { return worldMatrix().translation(); }
        auto localPosition() const -> Vector3 { return localPosition_; }

        auto worldUp() const -> Vector3 { return worldMatrix().upVector(); }
        auto localUp() const -> Vector3 { return matrix().upVector(); }

        auto worldDown() const -> Vector3 { return worldMatrix().downVector(); }
        auto localDown() const -> Vector3 { return matrix().downVector(); }

        auto worldLeft() const -> Vector3 { return worldMatrix().leftVector(); }
        auto localLeft() const -> Vector3 { return matrix().leftVector(); }

        auto worldRight() const -> Vector3 { return worldMatrix().rightVector(); }
        auto localRight() const -> Vector3 { return matrix().rightVector(); }

        auto worldForward() const -> Vector3 { return worldMatrix().forwardVector(); }
        auto localForward() const -> Vector3 { return matrix().forwardVector(); }

        auto worldBack() const -> Vector3 { return worldMatrix().backVector(); }
        auto localBack() const -> Vector3 { return matrix().backVector(); }

        void translateLocal(const Vector3 &translation);
        void scaleLocal(const Vector3 &scale);

        void setLocalPosition(const Vector3 &position);
        void setLocalScale(const Vector3 &scale);

        void rotate(const Quaternion &rotation, TransformSpace space = TransformSpace::Self);
        void rotateByAxisAngle(const Vector3 &axis, const Radians &angle, TransformSpace space = TransformSpace::Self);

        void setLocalRotation(const Quaternion &rotation);
        void setLocalAxisAngleRotation(const Vector3 &axis, const Radians &angle);

        void lookAt(const Vector3 &target, const Vector3 &up);

        auto matrix() const -> Matrix;
        auto worldMatrix() const -> Matrix;
        auto worldViewMatrix(const Camera *camera) const -> Matrix;
        auto worldViewProjMatrix(const Camera *camera) const -> Matrix;
        auto invTransposedWorldViewMatrix(const Camera *camera) const -> Matrix;
        auto invTransposedWorldMatrix() const -> Matrix;

        auto transformPoint(const Vector3 &point) const -> Vector3;
        auto transformDirection(const Vector3 &direction) const -> Vector3;

    private:
        mutable u32 dirtyFlags_ = ~0;
        mutable u32 version_ = 0;

        Transform *parent_ = nullptr;
        vec<Transform *> children_;

        Vector3 localPosition_;
        Vector3 localScale_;
        Quaternion localRotation_;
        mutable Matrix matrix_;
        mutable Matrix worldMatrix_;
        mutable Matrix invTransposedWorldMatrix_;

        void setDirtyWithChildren(u32 flags) const;
        void setChildrenDirty(u32 flags) const;
    };
}
