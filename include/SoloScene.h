#pragma once

#include "SoloBase.h"

namespace solo
{
    class Component;
    class Node;
    class Device;

    class Scene
    {
    public:
        static shared<Scene> create(Device* device);

        SL_NONCOPYABLE(Scene);
        ~Scene();

        Device* getDevice() const;

        shared<Node> createNode();

        void addComponent(int nodeId, shared<Component> cmp);
        void addComponentWithTypeId(int nodeId, shared<Component> cmp, int typeId);

        void removeComponent(int nodeId, int typeId);
        void clearNodeComponents(int nodeId);
        void clear();

        Component* getComponent(int nodeId, int typeId) const;
        Component* findComponent(int nodeId, int typeId) const;

        void update();
        void render();

    private:
        struct ComponentInfo
        {
            shared<Component> component;
            bool removed;
        };

        using NodeComponentMap = std::unordered_map<int, ComponentInfo>;
        using AllComponentMap = std::unordered_map<int, NodeComponentMap>;

        explicit Scene(Device* device);

        template <class T>
        void updateRenderQueue(std::list<T>& queue, int componentTypeIdFilter);

        void removeComponent(int nodeId, NodeComponentMap::iterator cmpIt);
        void updateComponents();
        void clearRemovedComponents();

        Device* device;
        int nodeCounter = 0;
        bool cameraCacheDirty = true;
        bool clearAll = false;

        std::list<Component*> cameraQueue;
        std::list<Component*> renderQueue;

        AllComponentMap components;
        std::unordered_map<int, std::unordered_set<Component*>> componentsForRemoval;
    };

    inline Device* Scene::getDevice() const
    {
        return device;
    }
}
