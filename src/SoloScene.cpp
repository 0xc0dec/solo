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
    auto& nodeComponents = components.at(nodeId);
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
    for (auto& cmp : components.at(nodeId))
        cmp.second->terminate();
    components.erase(nodeId);
}


void Scene::clear()
{
    doClear = true;
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


template <class T>
void Scene::updateRenderQueue(std::list<T>& queue, size_t cmpTypeIdFilter)
{
    queue.clear();

    for (auto& nodeComponents : components)
    {
        auto nodeId = nodeComponents.first;
        for (auto& pair : nodeComponents.second)
        {
            auto component = pair.second.get();
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


void Scene::update()
{
    if (doClear)
    {
        while (!components.empty())
            removeAllComponents(components.begin()->first);
        doClear = false;
    }
    else
    {
        for (auto& node : components)
        {
            for (auto& component : node.second)
                component.second->update();
        }
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
            pair.second->onAfterCameraRender();
    }
}

