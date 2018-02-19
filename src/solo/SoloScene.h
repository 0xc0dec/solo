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
    class Camera;

    class Scene final: public NoCopyAndMove
    {
    public:
        static auto create(Device *device) -> sptr<Scene>;

		~Scene() = default;

        auto getDevice() const -> Device* { return device; }

        auto createNode() -> sptr<Node>;
        void removeNodeById(u32 nodeId);
        void removeNode(Node *node);

        auto findComponent(u32 nodeId, u32 typeId) const -> Component*;
        void addComponent(u32 nodeId, sptr<Component> cmp);
        void removeComponent(u32 nodeId, u32 typeId);

		void visit(const std::function<void(Component*)> &accept);
        void visitByTags(u32 tagMask, const std::function<void(Component*)> &accept);

    private:
        using NodeComponents = umap<u32, sptr<Component>>;
        using NodesWithComponents = umap<u32, NodeComponents>;

        struct ComponentContext
        {
            bool deleted = false;
            sptr<Component> component;
        };

        Device *device = nullptr;
        u32 nodeCounter = 0;
        umap<u32, umap<u32, ComponentContext>> nodes;
        umap<u32, uset<u32>> deletedComponents;
        vec<Camera*> cameras;

        explicit Scene(Device *device);

        void cleanupDeleted();
    };
}
