/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloNode.h"


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
        SL_DISABLE_COPY_AND_MOVE(Component)

        virtual ~Component() {}

        virtual auto getTypeId() -> uint32_t = 0;

        // Called when the component is added to the scene
        virtual void init() {}
        // Called when the component is removed from the scene
        virtual void terminate() {}
        
        virtual void update() {}
        virtual void render(const RenderContext &context) {}

        virtual void onComponentAdded(Component *cmp) {}
        virtual void onComponentRemoved(Component *cmp) {}

        auto getNode() const -> Node;

        auto getTag() const -> uint32_t;
        void setTag(uint32_t tag);

    protected:
        Node node;
        uint32_t tag = 1;

        explicit Component(const Node &node): node(node)
        {
        }
    };

    inline auto Component::getNode() const -> Node
    {
        return node;
    }

    inline auto Component::getTag() const -> uint32_t
    {
        return tag;
    }

    inline void Component::setTag(uint32_t tag)
    {
        this->tag = tag;
    }


    template <class T>
    class ComponentBase: public Component
    {
    public:
        static auto getId() -> uint32_t;

        explicit ComponentBase(const Node &node): Component(node) {}

        auto getTypeId() -> uint32_t override;
    };

    template <class T>
    auto ComponentBase<T>::getId() -> uint32_t
    {
        return ComponentTypeId::get<T>();
    }

    template <class T>
    auto ComponentBase<T>::getTypeId() -> uint32_t
    {
        return getId();
    }
}
