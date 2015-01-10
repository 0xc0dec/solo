#include "SoloTransform.h"
#include "SoloCamera.h"

using namespace solo;

const unsigned DIRTY_BIT_TRANSLATION = 1;
const unsigned DIRTY_BIT_ROTATION = 2;
const unsigned DIRTY_BIT_SCALE = 4;
const unsigned DIRTY_BIT_WORLD = 8;


Transform::Transform(size_t node):
	ComponentBase(node)
{
	_scale.set(Vector3::one());
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


const Matrix& Transform::getMatrix()
{
	if (_dirtyBits)
	{
		bool hasTranslation = !_translation.isZero();
		bool hasScale = !_scale.isOne();
		bool hasRotation = !_rotation.isIdentity();

		if (hasTranslation || isDirty<DIRTY_BIT_TRANSLATION>())
		{
			Matrix::createTranslation(_translation, &_matrix);
			if (hasRotation || isDirty<DIRTY_BIT_ROTATION>())
				_matrix.rotate(_rotation);
			if (hasScale || isDirty<DIRTY_BIT_SCALE>())
				_matrix.scale(_scale);
		}
		else if (hasRotation || isDirty<DIRTY_BIT_ROTATION>())
		{
			Matrix::createRotation(_rotation, &_matrix);
			if (hasScale || isDirty<DIRTY_BIT_SCALE>())
				_matrix.scale(_scale);
		}
		else if (hasScale || isDirty<DIRTY_BIT_SCALE>())
			Matrix::createScale(_scale, &_matrix);

		clean<DIRTY_BIT_TRANSLATION, DIRTY_BIT_ROTATION, DIRTY_BIT_SCALE>();
	}
	return _matrix;
}


const Matrix& Transform::getWorldMatrix()
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


const Matrix& Transform::getInverseTransposedWorldMatrix()
{
	if (checkAndCleanBit<DIRTY_BIT_WORLD>())
	{
		_inverseTransposedWorldMatrix = getWorldMatrix();
		_inverseTransposedWorldMatrix.invert();
		_inverseTransposedWorldMatrix.transpose();
	}
	return _inverseTransposedWorldMatrix;
}


Matrix Transform::getWorldViewMatrix(ptr<Camera> camera)
{
	Matrix result;
	Matrix::multiply(camera->getViewMatrix(), getWorldMatrix(), &result);
	return result;
}


Matrix Transform::getWorldViewProjectionMatrix(ptr<Camera> camera)
{
	Matrix result;
	Matrix::multiply(camera->getViewProjectionMatrix(), getWorldMatrix(), &result);
	return result;
}


Matrix Transform::getInverseTransposedWorldViewMatrix(ptr<Camera> camera)
{
	Matrix result;
	Matrix::multiply(camera->getViewMatrix(), getWorldMatrix(), &result);
	result.invert();
	result.transpose();
	return result;
}