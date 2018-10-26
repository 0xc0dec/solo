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
        Node(Scene *scene, u32 nodeId);

        auto getId() const -> u32 { return id_; }
        auto getScene() const -> Scene* { return scene_; }

        template <typename T>
        static auto findComponent(Scene *scene, u32 nodeId) -> T*;

        template <typename T, typename... Args>
        static auto addComponent(Scene *scene, u32 nodeId, Args &&... args) -> T*;

        template <typename T>
        static void removeComponent(Scene *scene, u32 nodeId);

        template <typename T>
        auto findComponent() const -> T*;

        template <typename T, typename... Args>
        auto addComponent(Args &&... args) -> T*;

        template <typename T>
        void removeComponent();

    private:
        Scene *scene_ = nullptr;
        u32 id_;
    };

    // Helper needed for making addComponent work with any kind of default components
    // (which require passing some parameters - simply specializing addComponent of Node doesn't work)
    template <class T>
    struct NodeHelper
    {
        template <class... Args>
        static auto addComponent(Scene *scene, u32 nodeId, Args &&... args) -> T *
        {
            auto cmp = std::make_shared<T>(Node(scene, nodeId), std::forward<Args>(args)...);
            auto base = std::static_pointer_cast<Component>(cmp);
            scene->addComponent(nodeId, base);
            return cmp.get();
        }
    };

    template <typename T, typename... Args>
    auto Node::addComponent(Scene *scene, u32 nodeId, Args &&... args) -> T *
    {
        return NodeHelper<T>::addComponent(scene, nodeId, std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    auto Node::addComponent(Args &&... args) -> T *
    {
        return addComponent<T>(scene_, id_, std::forward<Args>(args)...);
    }

    template <typename T>
    auto Node::findComponent(Scene *scene, u32 nodeId) -> T *
    {
        auto typeId = T::getId();
        auto cmp = scene->findComponent(nodeId, typeId);
        return static_cast<T *>(cmp);
    }

    template <typename T>
    auto Node::findComponent() const -> T *
    {
        return findComponent<T>(scene_, id_);
    }

    template <typename T>
    void Node::removeComponent(Scene *scene, u32 nodeId)
    {
        scene->removeComponent(nodeId, T::getId());
    }

    template <typename T>
    void Node::removeComponent()
    {
        removeComponent<T>(scene_, id_);
    }
}
