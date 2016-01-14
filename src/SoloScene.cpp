#include "SoloScene.h"
#include "SoloComponent.h"
#include "SoloRenderContext.h"
#include "SoloCamera.h"
#include "SoloNode.h"
#include <algorithm>

using namespace solo;


shared<Scene> Scene::create(Device* device)
{
    return SL_NEW_SHARED(Scene, device);
}


Scene::Scene(Device* device) :
    device(device)
{
}


Scene::~Scene()
{
    // Allow components to do some cleanup work
    clear();
    clearRemovedComponents();
}


shared<Node> Scene::createNode()
{
    auto node = SL_NEW_SHARED(Node, this, nodeCounter++);
    node->addComponent<Transform>();
    return node;
}


void Scene::addComponent(size_t nodeId, shared<Component> cmp)
{
    addComponentWithTypeId(nodeId, cmp, cmp->getTypeId());
}


void Scene::addComponentWithTypeId(size_t nodeId, shared<Component> cmp, size_t typeId)
{
    auto nodeIt = components.find(nodeId);
    if (nodeIt != components.end())
    {
        auto& nodeComponents = nodeIt->second;
        auto existing = nodeComponents.find(typeId);
        if (existing != nodeComponents.end() && !existing->second.removed)
            SL_THROW_FMT(EngineException, "Component ", typeId, " already exists");
    }

    components[nodeId][typeId] = { cmp, false };
    cmp->init();

    if (typeId == Camera::getId())
        cameraCacheDirty = true;
}


void Scene::removeComponent(size_t nodeId, size_t typeId)
{
    auto nodeIt = components.find(nodeId);
    if (nodeIt != components.end())
    {
        auto cmpIt = nodeIt->second.find(typeId);
        if (cmpIt != nodeIt->second.end())
            removeComponent(nodeId, cmpIt);
    }
}


void Scene::removeComponent(int nodeId, NodeComponentMap::iterator cmpIt)
{
    if (cmpIt->second.removed)
        return;
    componentsForRemoval[nodeId].insert(cmpIt->second.component.get());
    cmpIt->second.removed = true;
}


void Scene::clearNodeComponents(size_t nodeId)
{
    auto nodeIt = components.find(nodeId);
    if (nodeIt != components.end())
    {
        for (auto cmpIt = nodeIt->second.begin(); cmpIt != nodeIt->second.end(); ++cmpIt)
            removeComponent(nodeId, cmpIt);
    }
}


void Scene::clear()
{
    clearAll = true;
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
    auto nodeIt = components.find(nodeId);
    if (clearAll || nodeIt == components.end())
        return nullptr;

    auto& nodeComponents = nodeIt->second;
    auto cmpIt = nodeComponents.find(typeId);
    if (cmpIt != nodeComponents.end() && !cmpIt->second.removed)
        return cmpIt->second.component.get();

    return nullptr;
}


template <class T>
void Scene::updateRenderQueue(std::list<T>& queue, size_t cmpTypeIdFilter)
{
    queue.clear();

    for (auto& nodeComponents : components)
    {
        auto nodeId = nodeComponents.first;
        for (auto& pair : nodeComponents.second)
        {
            auto component = pair.second.component.get();
            if (cmpTypeIdFilter > 0 && component->getTypeId() != cmpTypeIdFilter)
                continue;

            if (component->getRenderQueue() == KnownRenderQueues::NotRendered)
                continue;

            auto transform = Node::findComponent<Transform>(this, nodeId);
            if (!transform) // TODO save this transform for later use
                continue;

            for (auto it = queue.begin();; ++it)
            {
                if (it == queue.end() || component->getRenderQueue() < (*it)->getRenderQueue())
                {
                    queue.insert(it, component);
                    break;
                }
            }
        }
    }
}


void Scene::updateComponents()
{
    for (auto& node : components)
    {
        for (auto& component : node.second)
        {
            // Once clearAll is set to true, we stop updating the rest of the nodes
            if (!clearAll && !component.second.removed)
                component.second.component->update();
        }
    }
}


void Scene::clearRemovedComponents()
{
    if (clearAll)
    {
        for (auto& node : components)
        {
            for (auto& component : node.second)
                component.second.component->terminate();
        }
        components.clear();
        clearAll = false;
    }
    else
    {
        // TODO what if terminate() adds or removes another component?
        for (auto& node : componentsForRemoval)
        {
            for (auto& component : node.second)
            {
                component->terminate();
                if (component->getTypeId() == Camera::getId())
                    cameraCacheDirty = true;

                auto& nodeComponents = components.at(node.first);
                nodeComponents.erase(component->getTypeId());

                if (nodeComponents.empty())
                    components.erase(node.first);
            }
        }
    }

    componentsForRemoval.clear();
}


void Scene::update()
{
    updateComponents();
    clearRemovedComponents();
}


bool tagsAreRenderable(const BitFlags& objectTags, const BitFlags& cameraTags)
{
    auto rawObjectTags = objectTags.getRaw();
    auto rawCameraTags = cameraTags.getRaw();
    return (rawObjectTags & rawCameraTags) != 0;
}


void Scene::render()
{
    updateRenderQueue(cameraQueue, Camera::getId());
    updateRenderQueue(renderQueue, 0);

    for (auto cam : cameraQueue)
    {
        auto camera = dynamic_cast<Camera*>(cam);

        camera->apply();

        RenderContext context;
        context.scene = this;
        context.camera = camera;

        auto renderTags = camera->getRenderTags();

        for (auto cmp : renderQueue)
        {
            if (tagsAreRenderable(cmp->getTags(), renderTags))
            {
                auto transform = Node::findComponent<Transform>(this, cmp->getNode().getId()); // TODO can be cached
                context.nodeTransform = transform;
                cmp->render(context);
            }
        }

        camera->finish();

        for (auto& pair : components.at(camera->getNode().getId()))
            pair.second.component->onAfterCameraRender();
    }
}

