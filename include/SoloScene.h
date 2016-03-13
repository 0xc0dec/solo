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

        void addComponent(uint32_t nodeId, shared<Component> cmp);
        void removeComponent(uint32_t nodeId, uint32_t typeId);

        Component* getComponent(uint32_t nodeId, uint32_t typeId) const;
        Component* findComponent(uint32_t nodeId, uint32_t typeId) const;

        void update();
        void render();

    private:
        using NodeComponentMap = std::unordered_map<uint32_t, shared<Component>>;
        using AllComponentMap = std::unordered_map<uint32_t, NodeComponentMap>;
        
        template <class T>
        void updateRenderQueue(std::list<T>& queue, uint32_t componentTypeIdFilter);
        
        void updateComponents();
        void rebuildComponentsToUpdate();

        Device* device;
        uint32_t nodeCounter = 0;
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
