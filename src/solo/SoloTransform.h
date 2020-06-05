/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloComponent.h"
#include "math/SoloVector3.h"
#include "math/SoloQuaternion.h"
#include "math/SoloMatrix.h"
#include "SoloNode.h"
#include "SoloEnums.h"

namespace solo
{
    class Camera;
    class Transform;
    struct Radians;

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
    	void setLocalScale(const Vector3 &scale);
    	void scaleLocal(const Vector3 &scale);

        auto worldRotation() const -> Quaternion { return worldMatrix().rotation(); }
		void setWorldRotation(const Quaternion &rotation);
    	
        auto localRotation() const -> Quaternion { return localRotation_; }
    	void setLocalRotation(const Quaternion &rotation);
        void setLocalAxisAngleRotation(const Vector3 &axis, const Radians &angle);

        auto worldPosition() const -> Vector3 { return worldMatrix().translation(); }
    	void setWorldPosition(const Vector3 &position);

    	auto localPosition() const -> Vector3 { return localPosition_; }
    	void setLocalPosition(const Vector3 &position);

    	void translateLocal(const Vector3 &translation);

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

        void rotate(const Quaternion &rotation, TransformSpace space = TransformSpace::Self);
        void rotateByAxisAngle(const Vector3 &axis, const Radians &angle, TransformSpace space = TransformSpace::Self);

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
    	mutable bool dirty_ = true;
        mutable u32 version_ = 0;

        Transform *parent_ = nullptr;
        vec<Transform *> children_;

        Vector3 localPosition_;
        Vector3 localScale_;
        Quaternion localRotation_;
        mutable Matrix matrix_;
        mutable Matrix worldMatrix_;
        mutable Matrix invTransposedWorldMatrix_;

        void setDirtyWithChildren() const;
        void setChildrenDirty() const;
    };
}
