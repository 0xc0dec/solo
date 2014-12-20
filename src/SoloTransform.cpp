#include "SoloTransform.h"

using namespace solo;


void Transform::addChild(ptr<ITransform> child)
{
	auto impl = scast<Transform>(child);
	if (impl->_parent.get() == this)
		return;
	if (impl->_parent)
		impl->_parent->removeChild(child);
	impl->_parent.reset(this);
	_children.push_back(impl);
}


void Transform::removeChild(ptr<ITransform> child)
{
	auto impl = scast<Transform>(child);
	if (impl->_parent.get() != this)
		return;
	_children.remove(impl);
}


void Transform::removeChildren()
{
	_children.clear();
}