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


void Scene::addComponent(uint32_t nodeId, sptr<Component> cmp)
{
    auto typeId = cmp->getTypeId();

    SL_PANIC_CHECK_BLOCK(
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
        a.second->onComponentAdded(cmp.get());
}


void Scene::removeComponent(uint32_t nodeId, uint32_t typeId)
{
    SL_PANIC_IF(typeId == Transform::getId(), "Transform component cannot be removed from a node"); // to simplify things

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
}


// TODO Make this more optimal.
// Currently this is the simplest way of making addComponent/removeComponent work from within visit().
// During the visit() call we keep iterating over the old version of nodes, so iterators stay valid.
// All changes will be seen only on the next iteration.
void Scene::visit(std::function<void(Component*)> accept)
{
    return visitByTags(~0, accept);
}


void Scene::visitByTags(uint32_t tagMask, std::function<void(Component*)> accept)
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
