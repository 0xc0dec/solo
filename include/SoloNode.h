/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloScene.h"


namespace solo
{
    // A convenient wrapper for working with components. There's no real "Node" in the engine
    class Node final
    {
    public:
        Node(Scene *scene, uint32_t nodeId);

        auto getId() const -> uint32_t;
        auto getScene() const -> Scene*;

        template <typename T>
        static auto findComponent(Scene *scene, uint32_t nodeId) -> T*;

        template <typename T, typename... Args>
        static auto addComponent(Scene *scene, uint32_t nodeId, Args &&... args) -> T*;

        template <typename T>
        static void removeComponent(Scene *scene, uint32_t nodeId);

        template <typename T>
        auto findComponent() const -> T*;

        template <typename T, typename... Args>
        auto addComponent(Args &&... args) -> T*;

        template <typename T>
        void removeComponent();

    private:
        Scene *scene = nullptr;
        uint32_t id;
    };

    // Helper needed for making addComponent work with any kind of default components
    // (which require passing some parameters - simply specializing addComponent of Node doesn't work)
    template <class T>
    struct NodeHelper
    {
        template <class... Args>
        static auto addComponent(Scene *scene, uint32_t nodeId, Args &&... args) -> T *
        {
            auto cmp = std::make_shared<T>(Node(scene, nodeId), std::forward<Args>(args)...);
            auto base = std::static_pointer_cast<Component>(cmp);
            scene->addComponent(nodeId, base);
            return cmp.get();
        }
    };

    inline auto Node::getId() const -> uint32_t
    {
        return id;
    }

    inline auto Node::getScene() const -> Scene *
    {
        return scene;
    }

    template <typename T, typename... Args>
    auto Node::addComponent(Scene *scene, uint32_t nodeId, Args &&... args) -> T *
    {
        return NodeHelper<T>::addComponent(scene, nodeId, std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    auto Node::addComponent(Args &&... args) -> T *
    {
        return addComponent<T>(scene, id, std::forward<Args>(args)...);
    }

    template <typename T>
    auto Node::findComponent(Scene *scene, uint32_t nodeId) -> T *
    {
        auto typeId = T::getId();
        auto cmp = scene->findComponent(nodeId, typeId);
        return static_cast<T *>(cmp);
    }

    template <typename T>
    auto Node::findComponent() const -> T *
    {
        return findComponent<T>(scene, id);
    }

    template <typename T>
    void Node::removeComponent(Scene *scene, uint32_t nodeId)
    {
        scene->removeComponent(nodeId, T::getId());
    }

    template <typename T>
    void Node::removeComponent()
    {
        removeComponent<T>(scene, id);
    }
}