#include "SoloSceneNode.h"
#include "SoloException.h"
#include "SoloCommonsInternal.h"

using namespace solo;


SceneNode::SceneNode(const str &name)
	: _name(name)
{
}


void SceneNode::addComponent(sptr<IComponent> cmp)
{
	if (findComponent(cmp->id()))
		throw EngineException("Component with same id already exists");
	_components[cmp->id()] = cmp;
}


sptr<IComponent> SceneNode::getComponent(const str &id)
{
	auto cmp = findComponent(id);
	if (!cmp)
		throw EngineException("Component not found");
	return cmp;
}


sptr<IComponent> SceneNode::findComponent(const str &id)
{
	auto it = _components.find(id);
	return it != _components.end() ? it->second : nullptr;
}


void SceneNode::update()
{
	for (auto cmp: _components)
		cmp.second->update();
}
