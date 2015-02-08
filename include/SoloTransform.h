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

		const Vector3& getScale() const;
		const Quaternion& getRotation() const;
		const Vector3& getPosition() const;
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

		void translate(const Vector3& translation);
		void translate(float x, float y, float z);

		void rotate(const Quaternion& rotation);
		void rotate(const Vector3& axis, float angle);

		void scale(float scale);
		void scale(const Vector3& scale);
		void scale(float x, float y, float z);

		void setPosition(const Vector3& position);
		void setPosition(float x, float y, float z);

		void setRotation(const Quaternion& rotation);
		void setRotation(const Vector3& axis, float angle);
		
		void setScale(float scale);
		void setScale(const Vector3& scale);
		void setScale(float x, float y, float z);

		Vector3 transformPoint(const Vector3& point) const;
		Vector3 transforDirection(const Vector3& direction) const;

	private:
		Transform(size_t node);
		Transform(const Transform& other);

		ptr<Transform> parent;
		std::list<ptr<Transform>> children;

		Vector3 localPosition;
		Vector3 localScale;
		Quaternion localRotation;
		mutable Matrix matrix;
		mutable Matrix worldMatrix;
		mutable Matrix inverseTransposedWorldMatrix;
		mutable Matrix inverseTransposedViewMatrix;
	};
}
