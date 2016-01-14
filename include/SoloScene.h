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

        void addComponent(size_t nodeId, shared<Component> cmp);
        void addComponentWithTypeId(size_t nodeId, shared<Component> cmp, size_t typeId);

        void removeComponent(size_t nodeId, size_t typeId);
        void clearNodeComponents(size_t nodeId);
        void clear();

        Component* getComponent(size_t nodeId, size_t typeId) const;
        Component* findComponent(size_t nodeId, size_t typeId) const;

        void update();
        void render();

    private:
        explicit Scene(Device* device);

        template <class T>
        void updateRenderQueue(std::list<T>& queue, size_t componentTypeIdFilter);

        Device* device;
        size_t nodeCounter = 0;
        bool cameraCacheDirty = true;
        bool doClear = false;

        std::list<Component*> cameraQueue;
        std::list<Component*> renderQueue;

        // nodeId -> {(componentTypeId, component), (...), ...}
        std::unordered_map<size_t, std::unordered_map<size_t, shared<Component>>> components;
    };

    inline Device* Scene::getDevice() const
    {
        return device;
    }
}
