#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloDirty.h"
#include "SoloMatrix.h"
#include "SoloVector3.h"
#include "SoloQuaternion.h"

namespace solo
{
	class Camera;
	class Node;

	class TransformCallback
	{
	public:
		virtual ~TransformCallback() {}
		
		virtual void onTransformChanged() = 0;
	};

	class Transform: public ComponentBase<Transform>, Dirty
	{
	public:
		enum class TransformSpace
		{
			Self,
			Parent,
			World
		};

		virtual ~Transform() override {}
		
		static ptr<Transform> create(Node* node);

		void addCallback(TransformCallback *callback);
		void removeCallback(TransformCallback *callback);

		void setParent(Transform *parent);
		Transform *getParent() const;
		Transform *getChild(size_t index) const;
		size_t getChildrenCount() const;
		void removeChildren();

		Vector3 getWorldScale() const;
		const Vector3& getLocalScale() const;

		Quaternion getWorldRotation() const;
		const Quaternion& getLocalRotation() const;

		Vector3 getWorldPosition() const;
		const Vector3& getLocalPosition() const;

		Vector3 getLocalUp() const;
		Vector3 getLocalDown() const;
		Vector3 getLocalLeft() const;
		Vector3 getLocalRight() const;
		Vector3 getLocalForward() const;
		Vector3 getLocalBack() const;

		void translateLocal(const Vector3& translation);
		void translateLocal(float x, float y, float z);

		void rotate(const Quaternion& rotation, TransformSpace space = TransformSpace::Self);
		void rotate(const Vector3& axis, float angleRadians, TransformSpace space = TransformSpace::Self);

		void scaleLocal(float scale);
		void scaleLocal(const Vector3& scale);
		void scaleLocal(float x, float y, float z);

		void setLocalPosition(const Vector3& position);
		void setLocalPosition(float x, float y, float z);

		void setLocalRotation(const Quaternion& rotation);
		void setLocalRotation(const Vector3& axis, float angleRadians);
		
		void setLocalScale(float scale);
		void setLocalScale(const Vector3& scale);
		void setLocalScale(float x, float y, float z);

		const Matrix& getMatrix() const;
		const Matrix& getWorldMatrix() const;
		const Matrix& getInverseTransposedWorldMatrix() const;

		Matrix getWorldViewMatrix(Camera* camera) const;
		Matrix getWorldViewProjectionMatrix(Camera* camera) const;
		Matrix getInverseTransposedWorldViewMatrix(Camera* camera) const;

		Vector3 transformPoint(const Vector3& point) const;
		Vector3 transforDirection(const Vector3& direction) const;

	private:
		Transform(Node* node);
		Transform(const Transform& other);
		Transform(Transform&& other);

		template <unsigned bit1, unsigned... bitN>
		void setDirtyWithChildren() const
		{
			setDirty<bit1, bitN...>();
			for (auto child : children)
				child->setDirtyWithChildren<bit1, bitN...>();
		}

		template <unsigned bit1, unsigned... bitN>
		void setChildrenDirty() const
		{
			for (auto child : children)
				child->setDirtyWithChildren<bit1, bitN...>();
		}

		Transform* parent;
		std::vector<Transform*> children;
		std::vector<TransformCallback*> callbacks;

		Vector3 localPosition;
		Vector3 localScale;
		Quaternion localRotation;
		mutable Matrix matrix;
		mutable Matrix worldMatrix;
		mutable Matrix inverseTransposedWorldMatrix;
		mutable Matrix inverseTransposedViewMatrix;
	};
}
