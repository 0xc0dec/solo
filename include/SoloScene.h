#pragma once

#include "SoloBase.h"
#include <unordered_map>
#include <list>


namespace solo
{
    class Component;
    class Node;
    class Device;

    class Scene final
    {
    public:
        explicit Scene(Device* device);
        ~Scene();

        SL_NONCOPYABLE(Scene)

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

        // TODO not cache-friendly
        AllComponentMap components;
        std::vector<shared<Component>> componentsToUpdate;
    };

    inline Device* Scene::getDevice() const
    {
        return device;
    }
}
