#pragma once

#include "SoloScene.h"


namespace solo
{
    class Transform;
    class Camera;
    class SkyboxRenderer;
    class Spectator;
    class MeshRenderer;

    // A convenient wrapper for working with components. There's no real "Node" in the engine
    class Node final
    {
    public:
        Node(Scene* scene, uint32_t nodeId);

        uint32_t getId() const;
        Scene* getScene() const;

        template <typename T, typename... Args>
        static T* addComponent(Scene* scene, uint32_t nodeId, Args... args);

        template <typename T, typename... Args>
        T* addComponent(Args... args);

        template <typename T>
        static T* getComponent(Scene* scene, uint32_t nodeId);

        template <typename T>
        T* getComponent();

        template <typename T>
        static T* findComponent(Scene* scene, uint32_t nodeId);

        template <typename T>
        T* findComponent() const;

        template <typename T>
        static void removeComponent(Scene* scene, uint32_t nodeId);

        template <typename T>
        void removeComponent();

    private:
        Scene* scene;
        uint32_t id;
    };


    inline uint32_t Node::getId() const
    {
        return id;
    }

    inline Scene* Node::getScene() const
    {
        return scene;
    }

    template <typename T, typename... Args>
    inline T* Node::addComponent(Scene* scene, uint32_t nodeId, Args... args)
    {
        auto cmp = SL_MAKE_SHARED<T>(Node(scene, nodeId), args...);
        auto base = SL_SHARED_STATIC_CAST<Component>(cmp);
        scene->addComponent(nodeId, base);
        return cmp.get();
    }

    template <typename T, typename... Args>
    inline T* Node::addComponent(Args... args)
    {
        return addComponent<T>(scene, id, args...);
    }

    template <typename T>
    inline T* Node::getComponent(Scene* scene, uint32_t nodeId)
    {
        auto typeId = T::getId();
        auto cmp = scene->getComponent(nodeId, typeId);
        return static_cast<T*>(cmp);
    }

    template <typename T>
    inline T* Node::getComponent()
    {
        return getComponent<T>(scene, id);
    }

    template <typename T>
    inline T* Node::findComponent(Scene* scene, uint32_t nodeId)
    {
        auto typeId = T::getId();
        auto cmp = scene->findComponent(nodeId, typeId);
        return static_cast<T*>(cmp);
    }

    template <typename T>
    inline T* Node::findComponent() const
    {
        return findComponent<T>(scene, id);
    }

    template <typename T>
    inline void Node::removeComponent(Scene* scene, uint32_t nodeId)
    {
        scene->removeComponent(nodeId, T::getId());
    }

    template <typename T>
    inline void Node::removeComponent()
    {
        removeComponent<T>(scene, id);
    }

    template<> Transform* Node::addComponent();
    template<> Camera* Node::addComponent();
    template<> MeshRenderer* Node::addComponent();
    template<> Spectator* Node::addComponent();
    template<> SkyboxRenderer* Node::addComponent();
}