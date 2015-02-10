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
			Local,
			Global
		};

		virtual ~Transform() override {}
		
		static ptr<Transform> create(size_t node);

		void addCallback(TransformCallback *callback);
		void removeCallback(TransformCallback *callback);

		void setParent(Transform *parent);
		Transform *getParent() const;
		Transform *getChild(size_t index) const;
		void removeChildren();
		void iterateChildren(std::function<void(Transform*)> action) const;
		size_t getChildrenCount() const;

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

		void rotate(const Quaternion& rotation, TransformSpace space = TransformSpace::Local);
		void rotate(const Vector3& axis, float angle, TransformSpace space = TransformSpace::Local);

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
