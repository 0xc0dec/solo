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
	_localScale.set(Vector3::one());
	setDirty<DIRTY_BIT_ALL>();
}


ptr<Transform> Transform::create(size_t node)
{
	return NEW2(Transform, node);
}


void Transform::addChild(ptr<Transform> child)
{
	if (child->_parent.get() == this)
		return;
	if (child->_parent)
		child->_parent->removeChild(child);
	child->_parent.reset(this);
	_children.push_back(child);
}


void Transform::removeChild(ptr<Transform> child)
{
	if (child->_parent.get() != this)
		return;
	_children.remove(child);
}


void Transform::removeChildren()
{
	_children.clear();
}


const Vector3& Transform::getScale() const
{
	return _localScale;
}


const Quaternion& Transform::getRotation() const
{
	return _localRotation;
}


const Matrix& Transform::getMatrix() const
{
	if (isDirty())
	{
		bool hasTranslation = !_localPosition.isZero();
		bool hasScale = !_localScale.isOne();
		bool hasRotation = !_localRotation.isIdentity();

		if (hasTranslation || isDirty<DIRTY_BIT_POSITION>())
		{
			Matrix::createTranslation(_localPosition, &_matrix);
			if (hasRotation || isDirty<DIRTY_BIT_ROTATION>())
				_matrix.rotate(_localRotation);
			if (hasScale || isDirty<DIRTY_BIT_SCALE>())
				_matrix.scale(_localScale);
		}
		else if (hasRotation || isDirty<DIRTY_BIT_ROTATION>())
		{
			Matrix::createRotation(_localRotation, &_matrix);
			if (hasScale || isDirty<DIRTY_BIT_SCALE>())
				_matrix.scale(_localScale);
		}
		else if (hasScale || isDirty<DIRTY_BIT_SCALE>())
			Matrix::createScale(_localScale, &_matrix);

		clean<DIRTY_BIT_POSITION, DIRTY_BIT_ROTATION, DIRTY_BIT_SCALE>();
	}
	return _matrix;
}


const Matrix& Transform::getWorldMatrix() const
{
	if (checkAndCleanBit<DIRTY_BIT_WORLD>())
	{
		if (_parent)
			Matrix::multiply(_parent->getWorldMatrix(), getMatrix(), &_worldMatrix);
		else
			_worldMatrix = getMatrix();
	}
	return _worldMatrix;
}


const Matrix& Transform::getInverseTransposedWorldMatrix() const
{
	if (checkAndCleanBit<DIRTY_BIT_WORLD>())
	{
		_inverseTransposedWorldMatrix = getWorldMatrix();
		_inverseTransposedWorldMatrix.invert();
		_inverseTransposedWorldMatrix.transpose();
	}
	return _inverseTransposedWorldMatrix;
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
	_localPosition += translation;
	setDirty<DIRTY_BIT_POSITION, DIRTY_BIT_WORLD>();
}


void Transform::translate(float x, float y, float z)
{
	_localPosition.x += x;
	_localPosition.y += y;
	_localPosition.z += z;
	setDirty<DIRTY_BIT_POSITION, DIRTY_BIT_WORLD>();
}


void Transform::rotate(const Quaternion& rotation)
{
	_localRotation *= rotation;
	setDirty<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
}


void Transform::rotate(const Vector3& axis, float angle)
{
	auto rotation = Quaternion::createFromAxisAngle(axis, angle);
	_localRotation *= rotation;
	_localRotation.normalize();
	setDirty<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
}


void Transform::scale(float scale)
{
	_localScale.scale(scale);
	setDirty<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::scale(const Vector3& scale)
{
	// TODO replace with *=
	_localScale.x *= scale.x;
	_localScale.y *= scale.y;
	_localScale.z *= scale.z;
	setDirty<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::scale(float x, float y, float z)
{
	_localScale.x *= x;
	_localScale.y *= y;
	_localScale.z *= z;
	setDirty<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::setScale(float scale)
{
	_localScale.set(scale, scale, scale);
	setDirty<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::setScale(const Vector3& scale)
{
	_localScale.set(scale);
	setDirty<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::setScale(float x, float y, float z)
{
	_localScale.set(x, y, z);
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
	_localRotation.set(rotation);
	setDirty<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
}


void Transform::setRotation(const Vector3& axis, float angle)
{
	_localRotation.set(axis, angle);
	setDirty<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
}


void Transform::setPosition(const Vector3& position)
{
	_localPosition.set(position);
	setDirty<DIRTY_BIT_POSITION, DIRTY_BIT_WORLD>();
}


void Transform::setPosition(float x, float y, float z)
{
	_localPosition.set(x, y, z);
	setDirty<DIRTY_BIT_POSITION, DIRTY_BIT_WORLD>();
}


const Vector3& Transform::getPosition() const
{
	return _localPosition;
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