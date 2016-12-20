/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloScene.h"
#include "SoloComponent.h"
#include "SoloRenderContext.h"
#include "SoloCamera.h"
#include "SoloNode.h"

using namespace solo;


Scene::Scene(Device *device, const FriendToken<Device> &):
    device(device)
{
}


auto Scene::createNode() -> sptr<Node>
{
    auto node = std::make_shared<Node>(this, nodeCounter++);
    node->addComponent<Transform>();
    return node;
}


void Scene::addComponent(uint32_t nodeId, sptr<Component> cmp)
{
    auto typeId = cmp->getTypeId();

    SL_BLOCK(
    {
        auto node = nodes.find(nodeId);
        if (node != nodes.end())
        {
            const auto &nodeComponents = node->second;
            SL_ERR_IF(nodeComponents.find(typeId) != nodeComponents.end(), "Node already contains component with same id")
        }
    });

    nodes[nodeId][typeId] = cmp;
    cmp->init();

    for (const auto &a : nodes.at(nodeId))
        a.second->onComponentAdded(cmp.get());

    componentsDirty = true;
    if (typeId == Camera::getId())
        cameraCacheDirty = true;
}


void Scene::removeComponent(uint32_t nodeId, uint32_t typeId)
{
    auto node = nodes.find(nodeId);
    if (node == nodes.end())
        return;

    auto cmpIt = node->second.find(typeId);
    if (cmpIt == node->second.end())
        return;

    auto cmp = cmpIt->second;
    node->second.erase(cmpIt);
    cmp->terminate();

    if (node->second.empty())
        nodes.erase(node);
    else
    {
        for (const auto &otherCmp : node->second)
            otherCmp.second->onComponentRemoved(cmp.get());
    }

    if (typeId == Camera::getId())
        cameraCacheDirty = true;
    componentsDirty = true;
}


auto Scene::findComponent(uint32_t nodeId, uint32_t typeId) const -> Component *
{
    auto node = nodes.find(nodeId);
    if (node == nodes.end())
        return nullptr;

    const auto &nodeComponents = node->second;
    auto cmpIt = nodeComponents.find(typeId);
    if (cmpIt != nodeComponents.end())
        return cmpIt->second.get();

    return nullptr;
}


void Scene::rebuildRenderQueue(std::list<Component *> &queue, std::function<bool(Component *)> ignoreComponent)
{
    queue.clear();

    for (const auto &node : nodes)
    {
        auto nodeId = node.first;

        if (!Node::findComponent<Transform>(this, nodeId))
            continue;

        for (const auto &components : node.second)
        {
            auto cmp = components.second.get();
            if (ignoreComponent(cmp))
                continue;

            if (cmp->getRenderQueue() == KnownRenderQueues::NotRendered)
                continue;

            for (auto it = queue.begin();; ++it)
            {
                if (it == queue.end() || cmp->getRenderQueue() < (*it)->getRenderQueue())
                {
                    queue.insert(it, cmp);
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
    for (const auto &node : nodes)
    {
        for (const auto &cmp : node.second)
            componentsToUpdate.push_back(cmp.second);
    }
}


void Scene::render()
{
    static auto ignoreNonCameras = [](Component * cmp)
    {
        return cmp->getTypeId() != Camera::getId();
    };
    static auto ignoreNone = [](Component * cmp)
    {
        return false;
    };

    rebuildRenderQueue(cameraQueue, ignoreNonCameras);
    rebuildRenderQueue(renderQueue, ignoreNone);

    for (auto cam : cameraQueue)
    {
        auto camera = dynamic_cast<Camera *>(cam);

        camera->apply();

        RenderContext context;
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

        for (const auto &pair : nodes.at(camera->getNode().getId()))
            pair.second->onAfterCameraRender();
    }
}

