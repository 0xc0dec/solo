#include "SoloScene.h"
#include "SoloIComponent.h"
#include "SoloSystem.h"
#include "SoloException.h"

using namespace solo;


Scene::Scene()
	: _nodeCounter(0)
{
}


size_t Scene::createNode()
{
	_nodeComponents[++_nodeCounter];
	return _nodeCounter;
}


bool Scene::nodeExists(size_t node)
{
	return _nodeComponents.find(node) != _nodeComponents.end();
}


void Scene::addComponent(size_t node, ptr<IComponent> cmp)
{
	_ensureNodeExists(node);
	if (findComponent(node, cmp->getTypeId()))
		throw EngineException("Component with same id already exists");
	_nodeComponents[node][cmp->getTypeId()] = cmp;
}


ptr<IComponent> Scene::getComponent(size_t node, size_t typeId)
{
	auto cmp = findComponent(node, typeId);
	if (!cmp)
		throw EngineException("Component not found"); // TODO include component id
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
	// TODO This might be optimised.
	for (auto system : _systems)
	{
		auto targetComponentId = system.first;
		for (auto node : _nodeComponents)
		{
			for (auto cmp : node.second)
			{
				if (cmp.second->getTypeId() == targetComponentId)
					system.second->update(node.first, cmp.second);
			}
		}
	}
}


void Scene::_ensureNodeExists(size_t node)
{
	if (!nodeExists(node))
		throw EngineException("Node not found"); // TODO include node id
}


Scene::Systems::iterator Scene::_findSystem(ptr<ISystem> system)
{
	return std::find_if(_systems.begin(), _systems.end(), [&](Systems::reference pair) -> bool { return pair.second == system; });
}


void Scene::_ensureSystemNotAdded(ptr<ISystem> system)
{
	if (systemAlreadyAdded(system))
		throw EngineException("System already added");
}


void Scene::addSystem(ptr<ISystem> system, size_t targetComponentTypeId)
{
	_ensureSystemNotAdded(system);
	_systems.push_back(std::make_pair(targetComponentTypeId, system));
}


void Scene::removeSystem(ptr<ISystem> system)
{
	auto it = _findSystem(system);
	if (it != _systems.end()) // really needed?
		_systems.erase(it);
}


bool Scene::systemAlreadyAdded(ptr<ISystem> system)
{
	return _findSystem(system) != _systems.end();
}
