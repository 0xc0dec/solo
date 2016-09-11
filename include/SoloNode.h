/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "SoloScene.h"


namespace solo
{
    struct SpecificDefaultComponent;

    // A convenient wrapper for working with components. There's no real "Node" in the engine
    class Node final
    {
    public:
        Node(Scene* scene, uint32_t nodeId);

        auto getId() const -> uint32_t;
        auto getScene() const -> Scene*;

        template <typename T>
        static auto findComponent(Scene* scene, uint32_t nodeId) -> T*;

        template <typename T, typename... Args>
        static auto addComponent(Scene* scene, uint32_t nodeId, Args&&... args) -> T*;

        template <typename T>
        static void removeComponent(Scene* scene, uint32_t nodeId);

        template <typename T>
        auto findComponent() const -> T*;

        template <typename T, typename... Args>
        auto addComponent(Args&&... args) -> T*;

        template <typename T>
        void removeComponent();

    private:
        Scene* scene;
        uint32_t id;
    };

    // Helper needed for making addComponent work with any kind of default components
    // (which require passing some parameters - simply specializing addComponent of Node doesn't work)
    template <class T>
    struct NodeHelper
    {
        template <class... Args>
        static auto addComponent(Scene* scene, uint32_t nodeId, Args&&... args) -> T*
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

    inline auto Node::getScene() const -> Scene*
    {
        return scene;
    }

    template <typename T, typename... Args>
    inline auto Node::addComponent(Scene* scene, uint32_t nodeId, Args&&... args) -> T*
    {
        return NodeHelper<T>::addComponent(scene, nodeId, std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    inline auto Node::addComponent(Args&&... args) -> T*
    {
        return addComponent<T>(scene, id, std::forward<Args>(args)...);
    }

    template <typename T>
    inline auto Node::findComponent(Scene* scene, uint32_t nodeId) -> T*
    {
        auto typeId = T::getId();
        auto cmp = scene->findComponent(nodeId, typeId);
        return static_cast<T*>(cmp);
    }

    template <typename T>
    inline auto Node::findComponent() const -> T*
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
}