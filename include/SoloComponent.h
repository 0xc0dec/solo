#pragma once

#include "SoloNode.h"
#include "SoloRenderQueue.h"


namespace solo
{
    struct RenderContext;

    struct ComponentTypeId
    {
        template <typename T>
        static auto get() -> uint32_t
        {
            static const auto id = counter++;
            return id;
        }

    private:
        static uint32_t counter;
    };


    class Component
    {
    public:
        SL_NONCOPYABLE(Component)
        virtual ~Component() {}

        virtual auto getTypeId() -> uint32_t = 0;

        virtual void init() {}
        virtual void update() {}
        virtual void terminate() {}
        virtual void render(RenderContext& context) {}
        virtual void onAfterCameraRender() {}

        auto getRenderQueue() const -> uint32_t;
        void setRenderQueue(uint32_t queue);

        auto getNode() const -> Node;

        auto getTags() -> uint32_t&;

    protected:
        explicit Component(const Node& node): node(node)
        {
        }

        Node node;
        uint32_t tags = 1;
        uint32_t renderQueue = KnownRenderQueues::NotRendered;
    };

    inline auto Component::getRenderQueue() const -> uint32_t
    {
        return renderQueue;
    }

    inline void Component::setRenderQueue(uint32_t queue)
    {
        renderQueue = queue;
    }

    inline auto Component::getNode() const -> Node
    {
        return node;
    }

    inline auto Component::getTags() -> uint32_t&
    {
        return tags;
    }


    template <class T>
    class ComponentBase: public Component
    {
    public:
        static auto getId() -> uint32_t;

        explicit ComponentBase(const Node& node): Component(node) {}

        virtual auto getTypeId() -> uint32_t override;
    };

    template <class T>
    inline auto ComponentBase<T>::getId() -> uint32_t
    {
        return ComponentTypeId::get<T>();
    }

    template <class T>
    inline auto ComponentBase<T>::getTypeId() -> uint32_t
    {
        return getId();
    }
}
