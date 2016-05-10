#include "SoloScene.h"
#include "SoloComponent.h"
#include "SoloRenderContext.h"
#include "SoloCamera.h"
#include "SoloNode.h"

using namespace solo;


auto Scene::createNode() -> sptr<Node>
{
    auto node = std::make_shared<Node>(this, nodeCounter++);
    node->addComponent<Transform>();
    return node;
}


void Scene::addComponent(uint32_t nodeId, sptr<Component> cmp)
{
    auto typeId = cmp->getTypeId();

    SL_IN_DEBUG(
    {
        auto nodeIt = components.find(nodeId);
        if (nodeIt != components.end())
        {
            auto& nodeComponents = nodeIt->second;
            if (nodeComponents.find(typeId) != nodeComponents.end())
                SL_FMT_THROW(InvalidOperationException, "Component ", typeId, " already exists");
        }
    })

    components[nodeId][typeId] = cmp;
    cmp->init();

    for (const auto& a : components.at(nodeId))
        a.second->onComponentAdded(cmp.get());

    componentsDirty = true;
    if (typeId == Camera::getId())
        cameraCacheDirty = true;
}


void Scene::removeComponent(uint32_t nodeId, uint32_t typeId)
{
    auto nodeIt = components.find(nodeId);
    if (nodeIt == components.end())
        return;
    auto cmpIt = nodeIt->second.find(typeId);
    if (cmpIt == nodeIt->second.end())
        return;
    
    auto cmp = cmpIt->second;
    nodeIt->second.erase(cmpIt);
    cmp->terminate();

    if (nodeIt->second.empty())
        components.erase(nodeIt);
    else
    {
        for (const auto& otherCmp : nodeIt->second)
            otherCmp.second->onComponentRemoved(cmp.get());
    }

    if (typeId == Camera::getId())
        cameraCacheDirty = true;
    componentsDirty = true;
}


auto Scene::findComponent(uint32_t nodeId, uint32_t typeId) const -> Component*
{
    auto nodeIt = components.find(nodeId);
    if (nodeIt == components.end())
        return nullptr;

    auto& nodeComponents = nodeIt->second;
    auto cmpIt = nodeComponents.find(typeId);
    if (cmpIt != nodeComponents.end())
        return cmpIt->second.get();

    return nullptr;
}


template <class T>
void Scene::updateRenderQueue(std::list<T>& queue, bool cameraQueue)
{
    queue.clear();

    for (auto& nodeComponents : components)
    {
        auto nodeId = nodeComponents.first;

        if (!Node::findComponent<Transform>(this, nodeId))
            continue;

        for (auto& pair : nodeComponents.second)
        {
            auto component = pair.second.get();
            if (cameraQueue && component->getTypeId() != Camera::getId())
                continue;

            if (component->getRenderQueue() == KnownRenderQueues::NotRendered)
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
    if (componentsDirty)
    {
        rebuildComponentsToUpdate();
        componentsDirty = false;
    }

    updateComponents();
}


void Scene::updateComponents()
{
    // TODO A component added during one of these update() methods
    // will be updated for the first time only on the next call to updateComponents(). That is intended.
    // TODO It's possible that a component can receive update() after it has been removed.
    // For example, a component can remove other component during update(), but the component
    // being removed sits later in this list.
    for (auto cmp : componentsToUpdate)
        cmp->update();
}


void Scene::rebuildComponentsToUpdate()
{
    componentsToUpdate.clear(); // TODO maybe overkill
    for (auto& nodeIt : components)
    {
        for (auto& cmpIt : nodeIt.second)
            componentsToUpdate.push_back(cmpIt.second);
    }
}


void Scene::render()
{
    updateRenderQueue(cameraQueue, true);
    updateRenderQueue(renderQueue, false);

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
            if (cmp->getTags() & renderTags)
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

