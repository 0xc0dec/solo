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

	class Transform: public ComponentBase<Transform>, Dirty
	{
	public:
		virtual ~Transform() override {}
		
		static ptr<Transform> create(size_t node);

		void addChild(ptr<Transform> child);
		void removeChild(ptr<Transform> child);
		void removeChildren();

		const Vector3& getLocalScale() const;
		const Quaternion& getLocalRotation() const;
		const Vector3& getLocalPosition() const;
		Vector3 getWorldPosition() const;

		Vector3 getUp() const;
		Vector3 getDown() const;
		Vector3 getLeft() const;
		Vector3 getRight() const;
		Vector3 getForward() const;
		Vector3 getBack() const;

		const Matrix& getMatrix() const;
		const Matrix& getWorldMatrix() const;
		const Matrix& getInverseTransposedWorldMatrix() const;

		Matrix getWorldViewMatrix(ptr<Camera> camera) const;
		Matrix getWorldViewProjectionMatrix(ptr<Camera> camera) const;
		Matrix getInverseTransposedWorldViewMatrix(ptr<Camera> camera) const;

		void rotate(const Quaternion& rotation);
		void rotate(const Vector3& axis, float angle);

		void scale(float scale);
		void scale(const Vector3& scale);
		// TODO setScale and so on

	private:
		Transform(size_t node);
		Transform(const Transform& other);

		ptr<Transform> _parent;
		std::list<ptr<Transform>> _children;

		Vector3 _localPosition;
		Vector3 _localScale;
		Quaternion _localRotation;
		mutable Matrix _matrix;
		mutable Matrix _worldMatrix;
		mutable Matrix _inverseTransposedWorldMatrix;
		mutable Matrix _inverseTransposedViewMatrix;
	};
}
