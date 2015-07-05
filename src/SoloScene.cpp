#include "SoloScene.h"
#include "SoloComponent.h"
#include "SoloException.h"
#include "SoloRenderContext.h"
#include "SoloCamera.h"
#include "SoloNode.h"

using namespace solo;


Node* Scene::createEmptyNode()
{
	auto node = NodeFactory::createNode();
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


void Scene::addComponent(Node* node, shared<Component> cmp)
{
	addComponent(node, cmp, cmp->getTypeId());
}


void Scene::addComponent(Node* node, shared<Component> cmp, size_t typeId)
{
	auto nodeId = node->getId();
	ensureNodeExists(nodeId);
	if (findComponent(node, typeId))
		THROW_FMT(EngineException, "Component ", cmp->getTypeId(), " already exists.");
	components[nodeId][typeId] = cmp;
}


void Scene::removeComponent(Node* node, size_t typeId)
{
	auto nodeId = node->getId();
	ensureNodeExists(nodeId);
	components[nodeId].erase(typeId);
	if (components.at(nodeId).empty())
		nodes.erase(nodeId);
}


Component* Scene::getComponent(Node* node, size_t typeId)
{
	auto cmp = findComponent(node, typeId);
	if (!cmp)
		THROW_FMT(EngineException, "Component ", typeId, " not found.");
	return cmp;
}


Component* Scene::findComponent(Node* node, size_t typeId)
{
	auto nodeId = node->getId();
	ensureNodeExists(nodeId);
	auto nodeComponents = components[nodeId];
	auto it = nodeComponents.find(typeId);
	return it != nodeComponents.end() ? it->second.get() : nullptr;
}


std::vector<shared<Camera>> Scene::getCameras()
{
	std::vector<shared<Camera>> result;
	result.reserve(10);
	for (auto nodeComponents : components)
	{
		for (auto component : nodeComponents.second)
		{
			if (component.second->getTypeId() == Camera::getId())
				result.push_back(STATIC_CAST<Camera>(component.second));
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
		RenderContext context;
		context.setCameraNode(camera->getNode());
		camera->render(context);
		iterateComponents([&](shared<Node> node, shared<Component> component)
		{
			if (node->findComponent<Transform>())  // not very optimal - could be done one level upper
			{
				context.setNode(node.get());
				// Render only non-camera nodes with transforms
				if (component->getTypeId() != Camera::getId())
					component->render(context);
			}
		});
		camera->postRender();
		iterateComponents([&](shared<Node> node, shared<Component> component)
		{
			if (node->findComponent<Transform>())  // not very optimal - could be done one level upper
			{
				if (component->getTypeId() != Camera::getId())
					component->postRender();
			}
		});
	}
}


void Scene::iterateComponents(std::function<void(shared<Node>, shared<Component>)> work)
{
	for (auto nodeInfo : nodes)
	{
		auto node = nodeInfo.second;
		for (auto component : components[nodeInfo.first])
			work(node, component.second);
	}
}


void Scene::ensureNodeExists(size_t nodeId)
{
	if (nodes.find(nodeId) == nodes.end())
		THROW_FMT(EngineException, "Node ", nodeId, " not found.");
}


shared<Scene> SceneFactory::create()
{
	return NEW2(Scene);
}
