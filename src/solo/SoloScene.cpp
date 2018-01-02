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

void Scene::cleanupDeleted()
{
    // "Garbage collect" deleted stuff
    for (auto &nodeComponents: deletedComponents)
    {
        auto nodeId = nodeComponents.first;
        for (auto cmpId: nodeComponents.second)
            nodes.at(nodeId).erase(cmpId);
        if (nodes.at(nodeId).empty())
            nodes.erase(nodeId);
    }

    deletedComponents.clear();
}

auto Scene::createNode() -> sptr<Node>
{
    auto node = std::make_shared<Node>(this, nodeCounter++);
    node->addComponent<Transform>();
    return node;
}

void Scene::removeNodeById(u32 nodeId)
{
    if (nodes.count(nodeId))
    {
        for (auto &component: nodes.at(nodeId))
            removeComponent(nodeId, component.second.component->getTypeId());
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
            SL_PANIC_IF(nodeComponents.find(typeId) != nodeComponents.end(),
                    "Node already contains component with same id");
        }
    });

    nodes[nodeId][typeId].component = cmp;
    cmp->init();
}

void Scene::removeComponent(u32 nodeId, u32 typeId)
{
    auto node = nodes.find(nodeId);
    if (node == nodes.end())
        return;

    auto &nodeComponents = node->second;

    const auto cmpIt = nodeComponents.find(typeId);
    if (cmpIt == nodeComponents.end() || cmpIt->second.deleted)
        return;

    auto cmp = cmpIt->second;
    cmp.deleted = true;
    cmp.component->terminate();

    deletedComponents[nodeId].insert(typeId);
}

void Scene::visit(std::function<void(Component*)> accept)
{
    return visitByTags(~0, accept);
}

void Scene::visitByTags(u32 tagMask, std::function<void(Component*)> accept)
{
    for (const auto &node: nodes)
    {
        for (const auto &cmp : node.second)
        {
            if (!cmp.second.deleted && cmp.second.component->getTag() & tagMask)
                accept(cmp.second.component.get());
        }
    }

    cleanupDeleted();
}

auto Scene::findComponent(u32 nodeId, u32 typeId) const -> Component*
{
    const auto node = nodes.find(nodeId);
    if (node == nodes.end())
        return nullptr;

    const auto &nodeComponents = node->second;
    const auto cmpIt = nodeComponents.find(typeId);
    if (cmpIt != nodeComponents.end() && !cmpIt->second.deleted)
        return cmpIt->second.component.get();

    return nullptr;
}
