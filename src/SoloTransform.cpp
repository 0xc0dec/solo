#include "SoloTransform.h"
#include "SoloCamera.h"

using namespace solo;

const unsigned DIRTY_BIT_POSITION = 1;
const unsigned DIRTY_BIT_ROTATION = 2;
const unsigned DIRTY_BIT_SCALE = 4;
const unsigned DIRTY_BIT_WORLD = 8; // TODO needed?
const unsigned DIRTY_BIT_ALL = DIRTY_BIT_POSITION | DIRTY_BIT_ROTATION | DIRTY_BIT_SCALE | DIRTY_BIT_WORLD;


Transform::Transform(size_t node):
	ComponentBase(node)
{
	localScale.set(Vector3::one());
	setDirty<DIRTY_BIT_ALL>();
}


ptr<Transform> Transform::create(size_t node)
{
	return NEW2(Transform, node);
}


void Transform::addChild(ptr<Transform> child)
{
	if (child->parent.get() == this)
		return;
	if (child->parent)
		child->parent->removeChild(child);
	child->parent.reset(this);
	children.push_back(child.get());
}


void Transform::removeChild(ptr<Transform> child)
{
	if (child->parent.get() != this)
		return;
	children.erase(std::remove(children.begin(), children.end(), child.get()), children.end());
}


void Transform::removeChildren()
{
	children.clear();
}


const Vector3& Transform::getScale() const
{
	return localScale;
}


const Quaternion& Transform::getRotation() const
{
	return localRotation;
}


const Matrix& Transform::getMatrix() const
{
	if (isDirty())
	{
		auto hasTranslation = !localPosition.isZero();
		auto hasScale = !localScale.isOne();
		auto hasRotation = !localRotation.isIdentity();

		if (hasTranslation || isDirty<DIRTY_BIT_POSITION>())
		{
			Matrix::createTranslation(localPosition, &matrix);
			if (hasRotation || isDirty<DIRTY_BIT_ROTATION>())
				matrix.rotate(localRotation);
			if (hasScale || isDirty<DIRTY_BIT_SCALE>())
				matrix.scale(localScale);
		}
		else if (hasRotation || isDirty<DIRTY_BIT_ROTATION>())
		{
			Matrix::createRotation(localRotation, &matrix);
			if (hasScale || isDirty<DIRTY_BIT_SCALE>())
				matrix.scale(localScale);
		}
		else if (hasScale || isDirty<DIRTY_BIT_SCALE>())
			Matrix::createScale(localScale, &matrix);

		clean<DIRTY_BIT_POSITION, DIRTY_BIT_ROTATION, DIRTY_BIT_SCALE>();
	}
	return matrix;
}


const Matrix& Transform::getWorldMatrix() const
{
	if (checkAndCleanBit<DIRTY_BIT_WORLD>())
	{
		if (parent)
			Matrix::multiply(parent->getWorldMatrix(), getMatrix(), &worldMatrix);
		else
			worldMatrix = getMatrix();
	}
	return worldMatrix;
}


const Matrix& Transform::getInverseTransposedWorldMatrix() const
{
	if (checkAndCleanBit<DIRTY_BIT_WORLD>())
	{
		inverseTransposedWorldMatrix = getWorldMatrix();
		inverseTransposedWorldMatrix.invert();
		inverseTransposedWorldMatrix.transpose();
	}
	return inverseTransposedWorldMatrix;
}


Matrix Transform::getWorldViewMatrix(ptr<Camera> camera) const
{
	Matrix result;
	Matrix::multiply(camera->getViewMatrix(), getWorldMatrix(), &result);
	return result;
}


Matrix Transform::getWorldViewProjectionMatrix(ptr<Camera> camera) const
{
	Matrix result;
	Matrix::multiply(camera->getViewProjectionMatrix(), getWorldMatrix(), &result);
	return result;
}


Matrix Transform::getInverseTransposedWorldViewMatrix(ptr<Camera> camera) const
{
	Matrix result;
	Matrix::multiply(camera->getViewMatrix(), getWorldMatrix(), &result);
	result.invert();
	result.transpose();
	return result;
}


void Transform::translate(const Vector3& translation)
{
	localPosition += translation;
	setDirty<DIRTY_BIT_POSITION, DIRTY_BIT_WORLD>();
}


void Transform::translate(float x, float y, float z)
{
	localPosition.x += x;
	localPosition.y += y;
	localPosition.z += z;
	setDirty<DIRTY_BIT_POSITION, DIRTY_BIT_WORLD>();
}


void Transform::rotate(const Quaternion& rotation, TransformSpace space)
{
	// TODO use space
	localRotation *= rotation;
	localRotation.normalize();
	setDirty<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
}


void Transform::rotate(const Vector3& axis, float angle, TransformSpace space)
{
	// TODO use space
	auto rotation = Quaternion::createFromAxisAngle(axis, angle);
	localRotation *= rotation;
	localRotation.normalize();
	setDirty<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
}


void Transform::scale(float scale)
{
	localScale.scale(scale);
	setDirty<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::scale(const Vector3& scale)
{
	// TODO replace with *=
	localScale.x *= scale.x;
	localScale.y *= scale.y;
	localScale.z *= scale.z;
	setDirty<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::scale(float x, float y, float z)
{
	localScale.x *= x;
	localScale.y *= y;
	localScale.z *= z;
	setDirty<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::setScale(float scale)
{
	localScale.set(scale, scale, scale);
	setDirty<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::setScale(const Vector3& scale)
{
	localScale.set(scale);
	setDirty<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::setScale(float x, float y, float z)
{
	localScale.set(x, y, z);
	setDirty<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


Vector3 Transform::transformPoint(const Vector3& point) const
{
	return getMatrix().transformPoint(point);
}


Vector3 Transform::transforDirection(const Vector3& direction) const
{
	return getMatrix().transformDirection(direction);
}


void Transform::setRotation(const Quaternion& rotation)
{
	localRotation.set(rotation);
	setDirty<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
}


void Transform::setRotation(const Vector3& axis, float angle)
{
	localRotation.set(axis, angle);
	setDirty<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
}


void Transform::setPosition(const Vector3& position)
{
	localPosition.set(position);
	setDirty<DIRTY_BIT_POSITION, DIRTY_BIT_WORLD>();
}


void Transform::setPosition(float x, float y, float z)
{
	localPosition.set(x, y, z);
	setDirty<DIRTY_BIT_POSITION, DIRTY_BIT_WORLD>();
}


const Vector3& Transform::getPosition() const
{
	return localPosition;
}


Vector3 Transform::getWorldPosition() const
{
	return getWorldMatrix().getTranslation();
}


Vector3 Transform::getUp() const
{
	return getMatrix().getUpVector();
}


Vector3 Transform::getDown() const
{
	return getMatrix().getDownVector();
}


Vector3 Transform::getLeft() const
{
	return getMatrix().getLeftVector();
}


Vector3 Transform::getRight() const
{
	return getMatrix().getRightVector();
}


Vector3 Transform::getForward() const
{
	return getMatrix().getForwardVector();
}


Vector3 Transform::getBack() const
{
	return getMatrix().getBackVector();
}