/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
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

        auto getDevice() const -> Device* { return device; }

        auto createNode() -> sptr<Node>;
        void removeNodeById(u32 nodeId);
        void removeNode(Node *node);

        auto findComponent(u32 nodeId, u32 typeId) const -> Component*;
        void addComponent(u32 nodeId, sptr<Component> cmp);
        void removeComponent(u32 nodeId, u32 typeId);

        void visit(std::function<void(Component*)> accept);
        void visitByTags(u32 tagMask, std::function<void(Component*)> accept);

    private:
        using NodeComponents = umap<u32, sptr<Component>>;
        using NodesWithComponents = umap<u32, NodeComponents>;

        Device *device = nullptr;
        u32 nodeCounter = 0;
        NodesWithComponents nodes;

        explicit Scene(Device *device);
    };
}
