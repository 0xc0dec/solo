/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <unordered_map>
#include <list>
#include <functional>

namespace solo
{
    class Device;
    class Component;
    class Node;

    class Scene final
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Scene)

        static auto create(Device *device) -> sptr<Scene>;

        ~Scene() {}

        auto getDevice() const -> Device*;

        auto createNode() -> sptr<Node>;

        auto findComponent(uint32_t nodeId, uint32_t typeId) const -> Component*;
        void addComponent(uint32_t nodeId, sptr<Component> cmp);
        void removeComponent(uint32_t nodeId, uint32_t typeId);

        void visit(std::function<void(Component*)> accept);
        void visitByTags(uint32_t tagMask, std::function<void(Component*)> accept);

    private:
        using NodeComponents = std::unordered_map<uint32_t, sptr<Component>>;
        using NodesWithComponents = std::unordered_map<uint32_t, NodeComponents>;

        Device *device = nullptr;
        uint32_t nodeCounter = 0;
        NodesWithComponents nodes;

        explicit Scene(Device *device);
    };

    inline auto Scene::getDevice() const -> Device *
    {
        return device;
    }
}
