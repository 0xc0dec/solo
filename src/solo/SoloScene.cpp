/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloScene.h"
#include "SoloComponent.h"
#include "SoloTransform.h"
#include "SoloNode.h"
#include "SoloDevice.h"

using namespace solo;

auto Scene::create(Device *device) -> sptr<Scene>
{
    return sptr<Scene>(new Scene(device));
}

Scene::Scene(Device *device):
    device(device)
{
}

auto Scene::createNode() -> sptr<Node>
{
    auto node = std::make_shared<Node>(this, nodeCounter++);
    node->addComponent<Transform>();
    return node;
}

void Scene::removeNodeById(u32 nodeId)
{
    while (nodes.count(nodeId) && nodes.at(nodeId).size() > 1)
    {
        auto toRemove = nodes.at(nodeId).begin();
        // Remove Transform last because other components might be TransformCallbacks
        // TODO more generic approach
        if (toRemove->second->getTypeId() == Transform::getId())
            toRemove = ++nodes.at(nodeId).begin();
        removeComponent(nodeId, toRemove->second->getTypeId());
    }
}

void Scene::removeNode(Node *node)
{
    removeNodeById(node->getId());
}

void Scene::addComponent(u32 nodeId, sptr<Component> cmp)
{
    auto typeId = cmp->getTypeId();

    SL_PANIC_BLOCK(
    {
        auto node = nodes.find(nodeId);
        if (node != nodes.end())
        {
            const auto &nodeComponents = node->second;
            SL_PANIC_IF(nodeComponents.find(typeId) != nodeComponents.end(), "Node already contains component with same id");
        }
    });

    nodes[nodeId][typeId] = cmp;
    cmp->init();

    for (const auto &a : nodes.at(nodeId))
        a.second->handleComponentAdded(cmp.get());
}

void Scene::removeComponent(u32 nodeId, u32 typeId)
{
    auto node = nodes.find(nodeId);
    if (node == nodes.end())
        return;

    const auto cmpIt = node->second.find(typeId);
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
            otherCmp.second->handleComponentRemoved(cmp.get());
    }
}

// TODO Make this more optimal.
// Currently this is the simplest way of making addComponent/removeComponent work from within visit().
// During the visit() call we keep iterating over the old version of nodes, so iterators stay valid.
// All changes will be seen only on the next iteration.
void Scene::visit(std::function<void(Component*)> accept)
{
    return visitByTags(~0, accept);
}

void Scene::visitByTags(u32 tagMask, std::function<void(Component*)> accept)
{
    auto copy = nodes;
    for (const auto &node: copy)
    {
        for (const auto &cmp : node.second)
        {
            if (cmp.second->getTag() & tagMask) // i.e. component contains any tag from the mask
                accept(cmp.second.get());
        }
    }
}

auto Scene::findComponent(u32 nodeId, u32 typeId) const -> Component *
{
    const auto node = nodes.find(nodeId);
    if (node == nodes.end())
        return nullptr;

    const auto &nodeComponents = node->second;
    const auto cmpIt = nodeComponents.find(typeId);
    if (cmpIt != nodeComponents.end())
        return cmpIt->second.get();

    return nullptr;
}
