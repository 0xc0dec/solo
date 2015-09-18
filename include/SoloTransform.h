#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloDirty.h"
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

	class Transform: public ComponentBase<Transform>, Dirty
	{
	public:
		virtual void init() override;

		void addCallback(TransformCallback *callback);
		void removeCallback(TransformCallback *callback);

		void setParent(Transform *parent);
		Transform* getParent() const;
		Transform* getChild(size_t index) const;
		size_t getChildrenCount() const;
		void removeChildren();

		Vector3 getWorldScale() const;
		Vector3 getLocalScale() const;

		Quaternion getWorldRotation() const;
		Quaternion getLocalRotation() const;

		Vector3 getWorldPosition() const;
		Vector3 getLocalPosition() const;

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

		// Target and Up are in world coordinates
		void lookAt(const Vector3& target, const Vector3& up);

		Matrix getMatrix() const;
		Matrix getWorldMatrix() const;

		Matrix getWorldViewMatrix(Camera* camera) const;
		Matrix getWorldViewProjectionMatrix(Camera* camera) const;
		Matrix getInverseTransposedWorldViewMatrix(Camera* camera) const;
		Matrix getInverseTransposedWorldMatrix() const;

		Vector3 transformPoint(const Vector3& point) const;
		Vector3 transformDirection(const Vector3& direction) const;

	private:
		friend class TransformFactory;

		explicit Transform(Node node): ComponentBase(node) {}

		template <unsigned bit1, unsigned... bitN>
		void setDirtyWithChildren() const;

		template <unsigned bit1, unsigned... bitN>
		void setChildrenDirty() const;

		void notifyChanged() const;

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
		virtual void onTransformChanged(const Transform *transform) = 0;
	};

	class TransformFactory
	{
		friend class Node;
		static shared<Transform> create(Node node);
	};
}
