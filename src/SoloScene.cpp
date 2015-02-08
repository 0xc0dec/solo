#include "SoloScene.h"
#include "SoloComponent.h"
#include "SoloException.h"
#include "SoloRenderContext.h"
#include "SoloCamera.h"

using namespace solo;


Scene::Scene():
	nodeCounter(0)
{
}


ptr<Scene> Scene::create()
{
	return NEW2(Scene);
}


size_t Scene::createEmptyNode()
{
	components[++nodeCounter];
	return nodeCounter;
}


size_t Scene::createNode()
{
	auto node = createEmptyNode();
	addComponent<Transform>(node);
	return node;
}


bool Scene::nodeExists(size_t node)
{
	return components.find(node) != components.end();
}


void Scene::addComponent(size_t node, ptr<Component> cmp)
{
	ensureNodeExists(node);
	if (findComponent(node, cmp->getTypeId()))
		THROW(EngineException, "Component ", cmp->getTypeId(), " already exists");
	components[node][cmp->getTypeId()] = cmp;
}


void Scene::removeComponent(size_t node, size_t typeId)
{
	ensureNodeExists(node);
	components[node].erase(typeId);
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
	auto nodeComponents = components[node];
	auto it = nodeComponents.find(typeId);
	return it != nodeComponents.end() ? it->second : nullptr;
}


std::vector<ptr<Camera>> Scene::getCameras()
{
	std::vector<ptr<Camera>> result;
	result.reserve(10);
	for (auto nodeComponents : components)
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
	for (auto node : components)
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
		auto cameraTransform = getComponent<Transform>(camera->getNode());
		auto context = RenderContext(0, nullptr, camera, cameraTransform);
		camera->render(context);
		for (auto nodeComponents : components)
		{
			auto node = nodeComponents.first;
			for (auto component : nodeComponents.second)
			{
				auto nodeTransform = findComponent<Transform>(node);
				context.setNode(node, nodeTransform);
				// Ignore cameras - they're rendered in a special way
				if (nodeTransform && component.second->getTypeId() != Camera::getId())
					component.second->render(context);
			}
		}
	}
}


void Scene::ensureNodeExists(size_t node)
{
	if (!nodeExists(node))
		THROW(EngineException, "Node ", node, " not found");
}
