#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloMatrix.h"
#include "SoloVector3.h"
#include "SoloQuaternion.h"
#include "SoloNode.h"


namespace solo
{
    class Camera;
    class TransformCallback;

    enum class TransformSpace
    {
        Self,
        Parent,
        World
    };

    class Transform final: public ComponentBase<Transform>
    {
    public:
        explicit Transform(Node node);

        virtual void init() override final;

        void addCallback(TransformCallback* callback);
        void removeCallback(TransformCallback* callback);

        void setParent(Transform* parent);
        auto getParent() const -> Transform*;
        auto getChild(uint32_t index) const -> Transform*;
        auto getChildrenCount() const -> uint32_t;
        void removeChildren();

        auto getWorldScale() const -> Vector3;
        auto getLocalScale() const -> Vector3;

        auto getWorldRotation() const -> Quaternion;
        auto getLocalRotation() const -> Quaternion;

        auto getWorldPosition() const -> Vector3;
        auto getLocalPosition() const -> Vector3;

        auto getLocalUp() const -> Vector3;
        auto getLocalDown() const -> Vector3;
        auto getLocalLeft() const -> Vector3;
        auto getLocalRight() const -> Vector3;
        auto getLocalForward() const -> Vector3;
        auto getLocalBack() const -> Vector3;

        void translateLocal(const Vector3& translation);
        void scaleLocal(const Vector3& scale);

        void setLocalPosition(const Vector3& position);
        void setLocalScale(const Vector3& scale);

        void rotate(const Quaternion& rotation, TransformSpace space = TransformSpace::Self);
        void rotate(const Vector3& axis, float angleRadians, TransformSpace space = TransformSpace::Self);

        void setLocalRotation(const Quaternion& rotation);
        void setLocalRotation(const Vector3& axis, float angleRadians);

        void lookAt(const Vector3& target, const Vector3& up);

        auto getMatrix() const -> Matrix;
        auto getWorldMatrix() const -> Matrix;

        auto getWorldViewMatrix(Camera* camera) const -> Matrix;
        auto getWorldViewProjectionMatrix(Camera* camera) const -> Matrix;
        auto getInverseTransposedWorldViewMatrix(Camera* camera) const -> Matrix;
        auto getInverseTransposedWorldMatrix() const -> Matrix;

        auto transformPoint(const Vector3& point) const -> Vector3;
        auto transformDirection(const Vector3& direction) const -> Vector3;

    private:
        void setDirtyWithChildren(uint32_t flags) const;
        void setChildrenDirty(uint32_t flags) const;

        void notifyChanged() const;

        mutable uint32_t dirtyFlags;

        Transform* parent = nullptr;
        std::vector<Transform*> children;
        std::vector<TransformCallback*> callbacks;

        Vector3 localPosition;
        Vector3 localScale;
        Quaternion localRotation;
        mutable Matrix matrix;
        mutable Matrix worldMatrix;
        mutable Matrix inverseTransposedWorldMatrix;
    };

    class TransformCallback
    {
    public:
        virtual ~TransformCallback() {}
        virtual void onTransformChanged(const Transform* transform) = 0;
    };
}
