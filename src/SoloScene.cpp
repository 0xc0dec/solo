#include "SoloScene.h"
#include "SoloComponent.h"
#include "SoloException.h"
#include "SoloTransform.h"
#include "SoloCamera.h"

using namespace solo;


Scene::Scene():
	_nodeCounter(0)
{
}


ptr<Scene> Scene::create()
{
	return NEW2(Scene);
}


size_t Scene::createEmptyNode()
{
	_components[++_nodeCounter];
	return _nodeCounter;
}


size_t Scene::createNode()
{
	auto node = createEmptyNode();
	addComponent<Transform>(node);
	return node;
}


bool Scene::nodeExists(size_t node)
{
	return _components.find(node) != _components.end();
}


void Scene::addComponent(size_t node, ptr<Component> cmp)
{
	ensureNodeExists(node);
	if (findComponent(node, cmp->getTypeId()))
		THROW(EngineException, "Component ", cmp->getTypeId(), " already exists");
	_components[node][cmp->getTypeId()] = cmp;
}


void Scene::removeComponent(size_t node, size_t typeId)
{
	ensureNodeExists(node);
	_components[node].erase(typeId);
}


void Scene::removeComponent(size_t node, ptr<Component> cmp)
{
	ensureNodeExists(node);
	_components[node].erase(cmp->getTypeId());
}


ptr<Component> Scene::getComponent(size_t node, size_t typeId)
{
	auto cmp = findComponent(node, typeId);
	if (!cmp)
		THROW(EngineException, "Component ", typeId, " not found");
	return cmp;
}


ptr<Component> Scene::findComponent(size_t node, size_t typeId)
{
	ensureNodeExists(node);
	auto nodeComponents = _components[node];
	auto it = nodeComponents.find(typeId);
	return it != nodeComponents.end() ? it->second : nullptr;
}


std::vector<ptr<Camera>> Scene::getCameras()
{
	std::vector<ptr<Camera>> result;
	result.reserve(10);
	for (auto nodeComponents : _components)
	{
		for (auto component : nodeComponents.second)
		{
			if (component.second->getTypeId() == Camera::getId())
				result.push_back(CAST_PTR_STATIC<Camera>(component.second));
		}
	}
	return result;
}


void Scene::update()
{
	for (auto node : _components)
	{
		for (auto component : node.second)
			component.second->update();
	}
}


void Scene::render()
{
	auto cameras = getCameras(); // TODO cache lookup results or optimise in some other way
	for (auto camera : cameras)
	{
		camera->render();
		for (auto nodeComponents : _components)
		{
			for (auto component : nodeComponents.second)
			{
				// Ignore cameras - they're rendered in a special way
				if (component.second->getTypeId() != Camera::getId())
					component.second->render();
			}
		}
	}
}


void Scene::ensureNodeExists(size_t node)
{
	if (!nodeExists(node))
		THROW(EngineException, "Node ", node, " not found");
}
