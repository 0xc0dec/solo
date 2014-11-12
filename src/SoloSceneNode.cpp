#include "SoloSceneNode.h"
#include "SoloException.h"
#include "SoloCommonInternal.h"

using namespace solo;


SceneNode::SceneNode(const String &name)
	: _name(name)
{
}


void SceneNode::addComponent(ptr<IComponent> cmp)
{
	if (findComponent(cmp->id()))
		throw EngineException("Component with same id already exists");
	_components[cmp->id()] = cmp;
	_newComponents.push_back(cmp);
}


ptr<IComponent> SceneNode::getComponent(const String &id)
{
	auto cmp = findComponent(id);
	if (!cmp)
		throw EngineException("Component not found");
	return cmp;
}


ptr<IComponent> SceneNode::findComponent(const String &id)
{
	auto it = _components.find(id);
	return it != _components.end() ? it->second : nullptr;
}


void SceneNode::update()
{
	if (_newComponents.size() > 0)
	{
		for (auto cmp : _newComponents)
			cmp->start();
		_newComponents.clear();
	}

	for (auto cmp: _components)
		cmp.second->update();
}
