#pragma once

#include "SoloScene.h"


namespace solo
{
    class Transform;
    class Camera;
    class SkyboxRenderer;
    class Spectator;
    class MeshRenderer;
    class RigidBody;
    class FontRenderer;
    struct RigidBodyConstructionParameters;

    // A convenient wrapper for working with components. There's no real "Node" in the engine
    class Node final
    {
    public:
        Node(Scene* scene, uint32_t nodeId);

        auto getId() const -> uint32_t;
        auto getScene() const -> Scene*;

        template <typename T>
        static auto findComponent(Scene* scene, uint32_t nodeId) -> T*;

        template <typename T>
        static auto getComponent(Scene* scene, uint32_t nodeId) -> T*;

        template <typename T, typename... Args>
        static auto addComponent(Scene* scene, uint32_t nodeId, Args... args) -> T*;

        template <typename T>
        static void removeComponent(Scene* scene, uint32_t nodeId);

        template <typename T>
        auto findComponent() const -> T*;

        template <typename T>
        auto getComponent() -> T*;

        template <typename T, typename... Args>
        auto addComponent(Args... args) -> T*;

        template <typename T>
        void removeComponent();

    private:
        Scene* scene;
        uint32_t id;
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
    inline auto Node::addComponent(Scene* scene, uint32_t nodeId, Args... args) -> T*
    {
        auto cmp = std::make_shared<T>(Node(scene, nodeId), args...);
        auto base = std::static_pointer_cast<Component>(cmp);
        scene->addComponent(nodeId, base);
        return cmp.get();
    }

    template <typename T, typename... Args>
    inline auto Node::addComponent(Args... args) -> T*
    {
        return addComponent<T>(scene, id, args...);
    }

    template <typename T>
    inline auto Node::getComponent(Scene* scene, uint32_t nodeId) -> T*
    {
        auto typeId = T::getId();
        auto cmp = scene->findComponent(nodeId, typeId);
        if (!cmp)
            SL_FMT_THROW(InvalidOperationException, "Component ", typeId, " not found on node ", nodeId);
        return static_cast<T*>(cmp);
    }

    template <typename T>
    inline auto Node::getComponent() -> T*
    {
        return getComponent<T>(scene, id);
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

    template<> auto Node::addComponent() -> Transform*;
    template<> auto Node::addComponent() -> Camera*;
    template<> auto Node::addComponent() -> MeshRenderer*;
    template<> auto Node::addComponent() -> Spectator*;
    template<> auto Node::addComponent() -> SkyboxRenderer*;
    template<> auto Node::addComponent() -> FontRenderer*;
    template<> auto Node::addComponent(const RigidBodyConstructionParameters& parameters) -> RigidBody*;
}