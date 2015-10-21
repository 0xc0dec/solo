#include "SoloScene.h"
#include "SoloComponent.h"
#include "SoloRenderContext.h"
#include "SoloCamera.h"
#include "SoloNode.h"
#include "SoloRenderer.h"

using namespace solo;


shared<Scene> SceneFactory::create(Engine *engine)
{
	return SL_NEW2(Scene, engine);
}


Scene::Scene(Engine* engine) :
	engine{ engine }
{
}


Scene::~Scene()
{
	// Allow components to do some cleanup work
	clear();
}


Engine* Scene::getEngine()
{
	return engine;
}


shared<Node> Scene::createNode()
{
	auto node = SL_NEW2(Node, this, nodeCounter++);
	node->addComponent<Transform>();
	return node;
}


void Scene::addComponent(size_t nodeId, shared<Component> cmp)
{
	addComponent(nodeId, cmp, cmp->getTypeId());
}


void Scene::addComponent(size_t nodeId, shared<Component> cmp, size_t typeId)
{
	if (findComponent(nodeId, typeId))
		SL_THROW_FMT(EngineException, "Component ", typeId, " already exists");
	components[nodeId][typeId] = cmp;
	cmp->init();
	if (typeId == Camera::getId())
		cameraCacheDirty = true;
}


void Scene::removeComponent(size_t nodeId, size_t typeId)
{
	if (!findComponent(nodeId, typeId))
		return;
	auto &nodeComponents = components.at(nodeId);
	nodeComponents.at(typeId)->terminate();
	nodeComponents.erase(typeId);
	if (nodeComponents.empty())
		components.erase(nodeId);
	if (typeId == Camera::getId())
		cameraCacheDirty = true;
}


void Scene::removeAllComponents(size_t nodeId)
{
	if (components.find(nodeId) == components.end())
		return;
	for (auto& cmp: components.at(nodeId))
		cmp.second->terminate();
	components.erase(nodeId);
}


void Scene::clear()
{
	while (!components.empty())
		removeAllComponents(components.begin()->first);
}


Component* Scene::getComponent(size_t nodeId, size_t typeId) const
{
	auto cmp = findComponent(nodeId, typeId);
	if (!cmp)
		SL_THROW_FMT(EngineException, "Component ", typeId, " not found on node ", nodeId);
	return cmp;
}


Component* Scene::findComponent(size_t nodeId, size_t typeId) const
{
	if (components.find(nodeId) == components.end())
		return nullptr;
	auto nodeComponents = components.at(nodeId);
	auto it = nodeComponents.find(typeId);
	return it != nodeComponents.end() ? it->second.get() : nullptr;
}


void Scene::updateCameraCache()
{
	cameraCache.clear();
	for (auto& nodeComponents : components)
	{
		for (auto& component : nodeComponents.second)
		{
			if (component.second->getTypeId() == Camera::getId())
				cameraCache.push_back(static_cast<Camera*>(component.second.get()));
		}
	}
}


void Scene::update()
{
	for (auto& node : components)
	{
		for (auto& component : node.second)
			component.second->update();
	}
}


void Scene::render()
{
	if (cameraCacheDirty)
	{
		updateCameraCache();
		cameraCacheDirty = false;
	}

	RenderContext context;
	context.scene = this;

	for (auto& camera : cameraCache)
	{
		camera->apply();
		context.camera = camera;

		iterateComponents([&](size_t nodeId, shared<Component> component)
		{
			auto transform = Node::findComponent<Transform>(this, nodeId);
			if (transform)
			{
				auto renderer = dynamic_cast<Renderer*>(component.get());
				if (renderer)
				{
					context.nodeTransform = transform;
					renderer->render(context);
				}
			}
		});

		camera->finish();
	}
}


void Scene::iterateComponents(ComponentIterationWorker work)
{
	for (auto& nodeComponents : components)
	{
		auto nodeId = nodeComponents.first;
		for (auto& component : nodeComponents.second)
			work(nodeId, component.second);
	}
}
