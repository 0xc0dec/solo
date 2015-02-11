#include "SoloScene.h"
#include "SoloComponent.h"
#include "SoloException.h"
#include "SoloRenderContext.h"
#include "SoloCamera.h"
#include "SoloNode.h"

using namespace solo;


Scene::Scene():
	nodeCounter(0)
{
}


ptr<Scene> Scene::create()
{
	return NEW2(Scene);
}


Node* Scene::createEmptyNode()
{
	auto node = NEW2(Node, this);
	components[node->getId()];
	nodes[node->getId()] = node;
	return node.get();
}


Node* Scene::createNode()
{
	auto node = createEmptyNode();
	node->addComponent<Transform>();
	return node;
}


bool Scene::nodeExists(size_t nodeId)
{
	return nodes.find(nodeId) != nodes.end();
}


void Scene::addComponent(size_t nodeId, ptr<Component> cmp)
{
	ensureNodeExists(nodeId);
	if (findComponent(nodeId, cmp->getTypeId()))
		THROW(EngineException, "Component ", cmp->getTypeId(), " already exists");
	components[nodeId][cmp->getTypeId()] = cmp;
}


void Scene::removeComponent(size_t nodeId, size_t typeId)
{
	ensureNodeExists(nodeId);
	components[nodeId].erase(typeId);
	if (components.at(nodeId).empty())
		nodes.erase(nodeId);
}


ptr<Component> Scene::getComponent(size_t nodeId, size_t typeId)
{
	auto cmp = findComponent(nodeId, typeId);
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
		auto cameraTransform = camera->getNode()->getComponent<Transform>();
		RenderContext context(nullptr, nullptr, camera.get(), cameraTransform.get());
		camera->render(context);
		for (auto nodeInfo : nodes)
		{
			auto node = nodeInfo.second;
			for (auto component : components[nodeInfo.first])
			{
				auto nodeTransform = node->findComponent<Transform>();
				context.setNode(node.get(), nodeTransform.get());
				// Ignore cameras - they're rendered in a special way
				if (nodeTransform && component.second->getTypeId() != Camera::getId())
					component.second->render(context);
			}
		}
	}
}


void Scene::ensureNodeExists(size_t nodeId)
{
	if (!nodeExists(nodeId))
		THROW(EngineException, "Node ", nodeId, " not found");
}
