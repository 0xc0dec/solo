#include "SoloScene.h"
#include "SoloIComponent.h"
#include "SoloException.h"
#include "SoloTransform.h"
#include "SoloCamera.h"

using namespace solo;


Scene::Scene():
	_nodeCounter(0),
	_primaryCamera(nullptr)
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


ptr<ICamera> Scene::createCamera(size_t node)
{
	auto camera = NEW<Camera>();
	addComponent(node, camera);
	if (!_primaryCamera)
	{
		_primaryCamera = camera;
		_primaryCamera->setPrimary(true);
	}
	return camera;
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
		for (auto component : node.second)
			component.second->update();
	}
}


void Scene::render()
{
	if (_primaryCamera)
		_primaryCamera->render();
	// ...
}


void Scene::_ensureNodeExists(size_t node)
{
	if (!nodeExists(node))
		THROW(EngineException, "Node ", node, " not found");
}
