/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloScene.h"
#include "SoloComponent.h"
#include "SoloTransform.h"
#include "SoloNode.h"
#include "SoloDevice.h"
#include "SoloCamera.h"

using namespace solo;

auto Scene::empty(Device *device) -> sptr<Scene>
{
    return sptr<Scene>(new Scene(device));
}

Scene::Scene(Device *device):
    device_(device)
{
}

void Scene::cleanupDeleted()
{
    // "Garbage collect" deleted stuff
    for (auto &nodeComponents: deletedComponents_)
    {
        auto nodeId = nodeComponents.first;
        for (auto cmpId: nodeComponents.second)
            nodes_.at(nodeId).erase(cmpId);
        if (nodes_.at(nodeId).empty())
            nodes_.erase(nodeId);
    }

    deletedComponents_.clear();
}

auto Scene::createNode() -> sptr<Node>
{
    auto node = std::make_shared<Node>(this, nodeCounter_++);
    node->addComponent<Transform>();
    return node;
}

void Scene::removeNodeById(u32 nodeId)
{
    if (nodes_.count(nodeId))
    {
        for (auto &component: nodes_.at(nodeId))
            removeComponent(nodeId, component.second.component->typeId());
    }
}

void Scene::removeNode(Node *node)
{
    removeNodeById(node->id());
}

void Scene::addComponent(u32 nodeId, sptr<Component> cmp)
{
    const auto typeId = cmp->typeId();

    SL_DEBUG_BLOCK(
    {
        const auto node = nodes_.find(nodeId);
        if (node != nodes_.end())
        {
            const auto &nodeComponents = node->second;
            asrt(nodeComponents.find(typeId) == nodeComponents.end(), "Node already contains component with same id");
        }
    });

    nodes_[nodeId][typeId].component = cmp;
    cmp->init();

    if (cmp->typeId() == Camera::getId())
        cameras_.push_back(dynamic_cast<Camera*>(cmp.get()));
}

void Scene::removeComponent(u32 nodeId, u32 typeId)
{
    auto node = nodes_.find(nodeId);
    if (node == nodes_.end())
        return;

    auto &nodeComponents = node->second;

    const auto &cmpIt = nodeComponents.find(typeId);
    if (cmpIt == nodeComponents.end() || cmpIt->second.deleted)
        return;

    auto &cmp = cmpIt->second;
    cmp.deleted = true;
    cmp.component->terminate();

    deletedComponents_[nodeId].insert(typeId);

    if (cmp.component->typeId() == Camera::getId())
        cameras_.erase(std::remove(cameras_.begin(), cameras_.end(), cmp.component.get()));
}

void Scene::visit(const std::function<void(Component*)> &accept)
{
    return visitByTags(~0, accept);
}

void Scene::visitByTags(u32 tagMask, const std::function<void(Component*)> &accept)
{
    for (const auto &node: nodes_)
    {
        for (const auto &cmp : node.second)
        {
            if (!cmp.second.deleted && cmp.second.component->tag() & tagMask)
                accept(cmp.second.component.get());
        }
    }

    cleanupDeleted();
}

auto Scene::findComponent(u32 nodeId, u32 typeId) const -> Component*
{
    const auto node = nodes_.find(nodeId);
    if (node == nodes_.end())
        return nullptr;

    const auto &nodeComponents = node->second;
    const auto cmpIt = nodeComponents.find(typeId);
    if (cmpIt != nodeComponents.end() && !cmpIt->second.deleted)
        return cmpIt->second.component.get();

    return nullptr;
}
