#include "SoloScene.h"
#include "SoloComponent.h"
#include "SoloException.h"
#include "SoloRenderContext.h"
#include "SoloCamera.h"
#include "SoloNode.h"
#include "SoloModelRenderer.h"

using namespace solo;


shared<Scene> SceneFactory::create()
{
	return NEW2(Scene);
}


shared<Node> Scene::createNode()
{
	auto node = NEW2(Node, this, nodeCounter++);
	node->addComponent<Transform>();
	return node;
}


void Scene::clear()
{
	components.clear();
}


void Scene::addComponent(size_t nodeId, shared<Component> cmp)
{
	addComponent(nodeId, cmp, cmp->getTypeId());
}


void Scene::addComponent(size_t nodeId, shared<Component> cmp, size_t typeId)
{
	if (findComponent(nodeId, typeId))
		THROW_FMT(EngineException, "Component ", cmp->getTypeId(), " already exists.");
	components[nodeId][typeId] = cmp;
}


void Scene::removeComponent(size_t nodeId, size_t typeId)
{
	if (components.find(nodeId) == components.end())
		return;
	components[nodeId].erase(typeId);
	if (components.at(nodeId).empty())
		components.erase(nodeId);
}


void Scene::removeAllComponents(size_t nodeId)
{
	components.erase(nodeId);
}


Component* Scene::getComponent(size_t nodeId, size_t typeId)
{
	auto cmp = findComponent(nodeId, typeId);
	if (!cmp)
		THROW_FMT(EngineException, "Component ", typeId, " not found on node ", nodeId, ".");
	return cmp;
}


Component* Scene::findComponent(size_t nodeId, size_t typeId)
{
	if (components.find(nodeId) == components.end())
		return nullptr;
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
		context.setScene(this);
		context.setCameraNodeId(camera->getNodeId());
		camera->render(context);
		iterateComponents([&](size_t nodeId, shared<Component> component)
		{
			if (findComponent(nodeId, Transform::getId()))  // not very optimal - could be done one level upper
			{
				context.setNodeId(nodeId);
				// Render only non-camera nodes with transforms
				if (component->getTypeId() != Camera::getId())
					component->render(context);
			}
		});
		camera->postRender();
		iterateComponents([&](size_t nodeId, shared<Component> component)
		{
			if (findComponent(nodeId, Transform::getId()))  // not very optimal - could be done one level upper
			{
				if (component->getTypeId() != Camera::getId())
					component->postRender();
			}
		});
	}
}


void Scene::iterateComponents(ComponentInterationWorker work)
{
	for (auto nodeComponents : components)
	{
		auto nodeId = nodeComponents.first;
		for (auto component : nodeComponents.second)
			work(nodeId, component.second);
	}
}


template<> Transform* Scene::addComponent<Transform>(size_t nodeId)
{
	auto transform = TransformFactory::create(this, nodeId);
	addComponent(nodeId, transform);
	return transform.get();
}


template<> Camera* Scene::addComponent<Camera>(size_t nodeId)
{
	auto camera = CameraFactory::create(this, nodeId);
	addComponent(nodeId, camera);
	return camera.get();
}


template<> ModelRenderer* Scene::addComponent<ModelRenderer>(size_t nodeId)
{
	auto renderer = ModelRendererFactory::create(this, nodeId);
	addComponent(nodeId, renderer);
	return renderer.get();
}
