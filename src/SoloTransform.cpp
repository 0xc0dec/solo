#include "SoloTransform.h"
#include "SoloCamera.h"

using namespace solo;


const unsigned DIRTY_BIT_POSITION = 1;
const unsigned DIRTY_BIT_ROTATION = 2;
const unsigned DIRTY_BIT_SCALE = 4;
const unsigned DIRTY_BIT_WORLD = 8;
const unsigned DIRTY_BIT_INSERSE_TRANSPOSED_WORLD = 16;
const unsigned DIRTY_BIT_ALL =
	DIRTY_BIT_POSITION |
	DIRTY_BIT_ROTATION |
	DIRTY_BIT_SCALE |
	DIRTY_BIT_WORLD |
	DIRTY_BIT_INSERSE_TRANSPOSED_WORLD;


shared<Transform> TransformFactory::create(Node node)
{
	return NEW2(Transform, node);
}


void Transform::notifyChanged() const
{
	for (auto callback : callbacks)
		callback->onTransformChanged(this);
}


void Transform::init()
{
	localScale = Vector3::unit();
	setDirty<DIRTY_BIT_ALL>();
}


void Transform::addCallback(TransformCallback* callback)
{
	callbacks.push_back(callback);
}


void Transform::removeCallback(TransformCallback* callback)
{
	callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
}


void Transform::setParent(Transform* parent)
{
	if (parent == this || parent == this->parent)
		return;
	if (this->parent)
	{
		auto parentChildren = this->parent->children;
		this->parent->children.erase(std::remove(parentChildren.begin(), parentChildren.end(), this), parentChildren.end());
	}
	this->parent = parent;
	if (parent)
		parent->children.push_back(this);
	setDirtyWithChildren<DIRTY_BIT_WORLD>();
}


Transform* Transform::getParent() const
{
	return parent;
}


Transform* Transform::getChild(size_t index) const
{
	return children[index];
}


void Transform::removeChildren()
{
	while (!children.empty())
		(*children.begin())->setParent(nullptr);
}


size_t Transform::getChildrenCount() const
{
	return children.size();
}


Vector3 Transform::getLocalScale() const
{
	return localScale;
}


Quaternion Transform::getWorldRotation() const
{
	return getWorldMatrix().getRotation();
}


Vector3 Transform::getWorldScale() const
{
	return getWorldMatrix().getScale();
}


Quaternion Transform::getLocalRotation() const
{
	return getMatrix().getRotation();
}


Matrix Transform::getMatrix() const
{
	if (isDirty())
	{
		auto hasTranslation = !localPosition.isZero();
		auto hasScale = !localScale.isUnit();
		auto hasRotation = !localRotation.isIdentity();

		if (hasTranslation || isDirty<DIRTY_BIT_POSITION>())
		{
			matrix = Matrix::createTranslation(localPosition);
			if (hasRotation || isDirty<DIRTY_BIT_ROTATION>())
				matrix.rotate(localRotation);
			if (hasScale || isDirty<DIRTY_BIT_SCALE>())
				matrix.scale(localScale);
		}
		else if (hasRotation || isDirty<DIRTY_BIT_ROTATION>())
		{
			matrix = Matrix::createRotation(localRotation);
			if (hasScale || isDirty<DIRTY_BIT_SCALE>())
				matrix.scale(localScale);
		}
		else if (hasScale || isDirty<DIRTY_BIT_SCALE>())
			matrix = Matrix::createScale(localScale);

		clean<DIRTY_BIT_ALL>();
	}
	return matrix;
}


Matrix Transform::getWorldMatrix() const
{
	if (checkAndCleanBit<DIRTY_BIT_WORLD>())
	{
		if (parent)
			worldMatrix = parent->getWorldMatrix() * getMatrix();
		else
			worldMatrix = getMatrix();
		setChildrenDirty<DIRTY_BIT_WORLD>();
		setDirtyWithChildren<DIRTY_BIT_INSERSE_TRANSPOSED_WORLD>();
	}
	return worldMatrix;
}


Matrix Transform::getInverseTransposedWorldMatrix() const
{
	if (checkAndCleanBit<DIRTY_BIT_INSERSE_TRANSPOSED_WORLD>() || isDirty<DIRTY_BIT_WORLD>())
	{
		inverseTransposedWorldMatrix = getWorldMatrix();
		inverseTransposedWorldMatrix.invert();
		inverseTransposedWorldMatrix.transpose();
		setChildrenDirty<DIRTY_BIT_INSERSE_TRANSPOSED_WORLD>();
	}
	return inverseTransposedWorldMatrix;
}


Matrix Transform::getWorldViewMatrix(Camera* camera) const
{
	return camera->getViewMatrix() * getWorldMatrix();
}


Matrix Transform::getWorldViewProjectionMatrix(Camera* camera) const
{
	return camera->getViewProjectionMatrix() * getWorldMatrix();
}


Matrix Transform::getInverseTransposedWorldViewMatrix(Camera* camera) const
{
	auto result = camera->getViewMatrix() * getWorldMatrix();
	result.invert();
	result.transpose();
	return result;
}


void Transform::translateLocal(const Vector3& translation)
{
	localPosition += translation;
	setDirtyWithChildren<DIRTY_BIT_POSITION, DIRTY_BIT_WORLD>();
}


void Transform::translateLocal(float x, float y, float z)
{
	localPosition.x += x;
	localPosition.y += y;
	localPosition.z += z;
	setDirtyWithChildren<DIRTY_BIT_POSITION, DIRTY_BIT_WORLD>();
}


void Transform::rotate(const Quaternion& rotation, TransformSpace space)
{
	auto normalizedRotation(const_cast<Quaternion&>(rotation));
	normalizedRotation.normalize();

	switch (space)
	{
		case TransformSpace::Self:
			localRotation = localRotation * normalizedRotation;
			setDirtyWithChildren<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
			break;
		case TransformSpace::Parent:
			localRotation = normalizedRotation * localRotation;
			setDirtyWithChildren<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
			break;
		case TransformSpace::World:
		{
			auto inverseWorldRotation = getWorldRotation();
			inverseWorldRotation.inverse();
			localRotation = localRotation * inverseWorldRotation * normalizedRotation * getWorldRotation();
			setDirtyWithChildren<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
			break;
		}
	}
}


void Transform::rotate(const Vector3& axis, float angleRadians, TransformSpace space)
{
	auto rotation = Quaternion::createFromAxisAngle(axis, angleRadians);
	rotate(rotation, space);
}


void Transform::scaleLocal(float scale)
{
	localScale *= scale;
	setDirtyWithChildren<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::scaleLocal(const Vector3& scale)
{
	localScale.x *= scale.x;
	localScale.y *= scale.y;
	localScale.z *= scale.z;
	setDirtyWithChildren<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::scaleLocal(float x, float y, float z)
{
	localScale.x *= x;
	localScale.y *= y;
	localScale.z *= z;
	setDirtyWithChildren<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::setLocalScale(float scale)
{
	localScale = Vector3(scale, scale, scale);
	setDirtyWithChildren<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::setLocalScale(const Vector3& scale)
{
	localScale = scale;
	setDirtyWithChildren<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::setLocalScale(float x, float y, float z)
{
	localScale = Vector3(x, y, z);
	setDirtyWithChildren<DIRTY_BIT_SCALE, DIRTY_BIT_WORLD>();
}


void Transform::lookAt(const Vector3& target, const Vector3& up)
{
	auto finalTarget = target;
	auto finalUp = up;

	if (parent)
	{
		auto m(parent->getWorldMatrix());
		m.invert();
		finalTarget = m.transformPoint(target);
		finalUp = m.transformDirection(up);
	}

	Matrix lookAtMatrix = Matrix::createLookAt(localPosition, finalTarget, finalUp);
	setLocalRotation(lookAtMatrix.getRotation());
}


Vector3 Transform::transformPoint(const Vector3& point) const
{
	return getMatrix().transformPoint(point);
}


Vector3 Transform::transformDirection(const Vector3& direction) const
{
	return getMatrix().transformDirection(direction);
}


void Transform::setLocalRotation(const Quaternion& rotation)
{
	localRotation = rotation;
	setDirtyWithChildren<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
}


void Transform::setLocalRotation(const Vector3& axis, float angleRadians)
{
	localRotation = Quaternion::createFromAxisAngle(axis, angleRadians);
	setDirtyWithChildren<DIRTY_BIT_ROTATION, DIRTY_BIT_WORLD>();
}


void Transform::setLocalPosition(const Vector3& position)
{
	localPosition = position;
	setDirtyWithChildren<DIRTY_BIT_POSITION, DIRTY_BIT_WORLD>();
}


void Transform::setLocalPosition(float x, float y, float z)
{
	localPosition = Vector3(x, y, z);
	setDirtyWithChildren<DIRTY_BIT_POSITION, DIRTY_BIT_WORLD>();
}


Vector3 Transform::getLocalPosition() const
{
	return localPosition;
}


Vector3 Transform::getWorldPosition() const
{
	return getWorldMatrix().getTranslation();
}


Vector3 Transform::getLocalUp() const
{
	return getMatrix().getUpVector();
}


Vector3 Transform::getLocalDown() const
{
	return getMatrix().getDownVector();
}


Vector3 Transform::getLocalLeft() const
{
	return getMatrix().getLeftVector();
}


Vector3 Transform::getLocalRight() const
{
	return getMatrix().getRightVector();
}


Vector3 Transform::getLocalForward() const
{
	return getMatrix().getForwardVector();
}


Vector3 Transform::getLocalBack() const
{
	return getMatrix().getBackVector();
}


template <unsigned bit1, unsigned... bitN>
void Transform::setDirtyWithChildren() const
{
	setDirty<bit1, bitN...>();
	notifyChanged();
	for (auto child : children)
		child->setDirtyWithChildren<bit1, bitN...>();
}


template <unsigned bit1, unsigned... bitN>
void Transform::setChildrenDirty() const
{
	for (auto child : children)
		child->setDirtyWithChildren<bit1, bitN...>();
}
