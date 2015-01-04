#include "SoloScene.h"
#include "SoloIComponent.h"
#include "SoloException.h"
#include "SoloTransform.h"

using namespace solo;


Scene::Scene()
	: _nodeCounter(0)
{
}


size_t Scene::createEmptyNode()
{
	_nodeComponents[++_nodeCounter];
	return _nodeCounter;
}


size_t Scene::createNode()
{
	auto node = createEmptyNode();
	this->IScene::addComponent<Transform>(node);
	return node;
}


bool Scene::nodeExists(size_t node)
{
	return _nodeComponents.find(node) != _nodeComponents.end();
}


void Scene::addComponent(size_t node, ptr<IComponent> cmp)
{
	_ensureNodeExists(node);
	if (findComponent(node, cmp->getTypeId()))
		THROW(EngineException, "Component ", cmp->getTypeId(), " already exists");
	_nodeComponents[node][cmp->getTypeId()] = cmp;
}


ptr<IComponent> Scene::getComponent(size_t node, size_t typeId)
{
	auto cmp = findComponent(node, typeId);
	if (!cmp)
		THROW(EngineException, "Component ", typeId, " not found");
	return cmp;
}


ptr<IComponent> Scene::findComponent(size_t node, size_t typeId)
{
	_ensureNodeExists(node);
	auto nodeComponents = _nodeComponents[node];
	auto it = nodeComponents.find(typeId);
	return it != nodeComponents.end() ? it->second : nullptr;
}


void Scene::update()
{
	for (auto node : _nodeComponents)
	{
		for (auto cmp : node.second)
			cmp.second->update();
	}
}


void Scene::_ensureNodeExists(size_t node)
{
	if (!nodeExists(node))
		THROW(EngineException, "Node ", node, " not found");
}
