#include "SoloTransform.h"

using namespace solo;


Transform::Transform(size_t node):
	ComponentBase(node)
{
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
