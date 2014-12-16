#include "SoloScene.h"
#include "SoloIComponent.h"
#include "SoloException.h"

using namespace solo;


Scene::Scene()
	: _nodeCounter(0)
{
}


int Scene::createNode()
{
	_nodeComponents[++_nodeCounter];
	return _nodeCounter;
}


bool Scene::nodeExists(int node)
{
	return _nodeComponents.find(node) != _nodeComponents.end();
}


void Scene::addComponent(int node, ptr<IComponent> cmp)
{
	_ensureNodeExists(node);
	if (findComponent(node, cmp->id()))
		throw EngineException("Component with same id already exists");
	_nodeComponents[node][cmp->id()] = cmp;
}


ptr<IComponent> Scene::getComponent(int node, const std::string &id)
{
	auto cmp = findComponent(node, id);
	if (!cmp)
		throw EngineException("Component not found"); // TODO include component id
	return cmp;
}


ptr<IComponent> Scene::findComponent(int node, const std::string &id)
{
	_ensureNodeExists(node);
	auto nodeComponents = _nodeComponents[node];
	auto it = nodeComponents.find(id);
	return it != nodeComponents.end() ? it->second : nullptr;
}


void Scene::update()
{
	// TODO
}


void Scene::_ensureNodeExists(int node)
{
	if (!nodeExists(node))
		throw EngineException("Node not found"); // TODO include node id
}