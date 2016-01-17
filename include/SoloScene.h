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
        void removeComponent(int nodeId, int typeId);

        Component* getComponent(int nodeId, int typeId) const;
        Component* findComponent(int nodeId, int typeId) const;

        void update();
        void render();

    private:
        using NodeComponentMap = std::unordered_map<int, shared<Component>>;
        using AllComponentMap = std::unordered_map<int, NodeComponentMap>;

        explicit Scene(Device* device);

        template <class T>
        void updateRenderQueue(std::list<T>& queue, int componentTypeIdFilter);
        
        void updateComponents();
        void rebuildComponentsToUpdate();

        Device* device;
        int nodeCounter = 0;
        bool cameraCacheDirty = true;
        bool componentsDirty = true;

        std::list<Component*> cameraQueue;
        std::list<Component*> renderQueue;

        AllComponentMap components;
        std::vector<Component*> componentsToUpdate;
    };

    inline Device* Scene::getDevice() const
    {
        return device;
    }
}
