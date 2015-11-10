#include "SoloScene.h"
#include "SoloComponent.h"
#include "SoloRenderContext.h"
#include "SoloCamera.h"
#include "SoloNode.h"
#include <algorithm>

using namespace solo;


shared<Scene> Scene::create(Device *device)
{
	return SL_NEW2(Scene, device);
}


Scene::Scene(Device* device) :
	device{ device }
{
	
}


Scene::~Scene()
{
	// Allow components to do some cleanup work
	clear();
}


shared<Node> Scene::createNode()
{
	auto node = SL_NEW2(Node, this, nodeCounter++);
	node->addComponent<Transform>();
	return node;
}


void Scene::addComponent(size_t nodeId, shared<Component> cmp)
{
	addComponentWithTypeId(nodeId, cmp, cmp->getTypeId());
}


void Scene::addComponentWithTypeId(size_t nodeId, shared<Component> cmp, size_t typeId)
{
	if (findComponent(nodeId, typeId))
		SL_THROW_FMT(EngineException, "Component ", typeId, " already exists");
	components[nodeId][typeId] = cmp;
	cmp->init();
	if (typeId == Camera::getId())
		cameraCacheDirty = true;
	componentCount++;
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
	componentCount--;
}


void Scene::removeAllComponents(size_t nodeId)
{
	if (components.find(nodeId) == components.end())
		return;
	for (auto& cmp : components.at(nodeId))
	{
		cmp.second->terminate();
		componentCount--;
	}
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
	if (!cameraCacheDirty)
		return;

	cameraCache.clear();
	cameraCache.reserve(10); // hmm...
	for (auto& nodeComponents : components)
	{
		for (auto& component : nodeComponents.second)
		{
			if (component.second->getTypeId() == Camera::getId())
			{
				auto camera = static_cast<Camera*>(component.second.get());
				auto renderOrder = camera->getRenderOrder();
				auto whereToInsert = std::find_if(cameraCache.begin(), cameraCache.end(), [=](Camera *other)
				{
					return other->getRenderOrder() > renderOrder;
				});
				if (whereToInsert != cameraCache.begin())
					whereToInsert = whereToInsert--;
				cameraCache.insert(whereToInsert, camera);
			}
		}
	}

	cameraCacheDirty = false;
}


void Scene::updateRenderQueue()
{
	if (!componentCount)
		return;

	// TODO use more cache-friendly structure
	renderQueue.clear();

	iterateComponents([&](size_t nodeId, Component* component)
	{
		auto transform = Node::findComponent<Transform>(this, nodeId);
		if (!transform)
			return;

		for (auto it = renderQueue.begin();; ++it)
		{
			if (it == renderQueue.end() || component->getRenderQueue() < (*it)->getRenderQueue())
			{
				renderQueue.insert(it, component);
				break;
			}
		}
	});
}


void Scene::update()
{
	for (auto& node : components)
	{
		for (auto& component : node.second)
			component.second->update();
	}
}


bool tagsAreRenderable(const BitFlags& objectTags, const BitFlags& cameraTags)
{
	auto rawObjectTags = objectTags.getRaw();
	auto rawCameraTags = cameraTags.getRaw();
	return (rawObjectTags & rawCameraTags) != 0;
}


void Scene::render()
{
	updateCameraCache();
	for (auto& camera : cameraCache)
		renderWithCamera(camera);
}


void Scene::renderWithCamera(Camera* camera)
{
	// TODO avoid this if this has been already done in render()
	updateRenderQueue();

	camera->apply();

	RenderContext context;
	context.scene = this;
	context.camera = camera;

	auto renderTags = camera->getRenderTags();

	for (auto cmp: renderQueue)
	{
		auto transform = Node::findComponent<Transform>(this, cmp->getNode().getId()); // TODO can be cached
		// TODO not really needed because render queue already contains components
		// only for nodes with transforms
		if (transform && tagsAreRenderable(transform->getTags(), renderTags)) // TODO add render tags to all components
		{
			context.nodeTransform = transform;
			cmp->render(context);
		}
	}

//	iterateComponents([&](size_t nodeId, Component* component)
//	{
//		auto transform = Node::findComponent<Transform>(this, nodeId);
//		if (transform && tagsAreRenderable(transform->getTags(), renderTags))
//		{
//			context.nodeTransform = transform;
//			component->render(context);
//		}
//	});

	camera->finish();

	for (auto &pair: components[camera->getNode().getId()])
		pair.second->onAfterCameraRender();
}


void Scene::iterateComponents(ComponentIterationWorker work)
{
	for (auto& nodeComponents : components)
	{
		auto nodeId = nodeComponents.first;
		for (auto& component : nodeComponents.second)
			work(nodeId, component.second.get());
	}
}
