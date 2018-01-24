/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloNode.h"

namespace solo
{
    struct ComponentTypeId
    {
        template <typename T>
        static auto get() -> u32
        {
            static const auto id = counter++;
            return id;
        }

    private:
        static u32 counter;
    };

    class Component: public NoCopyAndMove
    {
    public:
        virtual ~Component() = default;

        virtual auto getTypeId() -> u32 = 0;

        virtual void init() {}
        virtual void terminate() {}
        
        virtual void update() {}
        virtual void render() {}

        auto getNode() const -> Node { return node; }

        auto getTag() const -> u32 { return tag; }
        void setTag(u32 tag) { this->tag = tag; }

        auto getLayer() const -> u32 { return layer; }
        void setLayer(u32 layer) { this->layer = layer; }

    protected:
        Node node;
        u32 tag = 1;
        u32 layer = 16;

        explicit Component(const Node &node): node(node)
        {
        }
    };

    template <class T>
    class ComponentBase: public Component
    {
    public:
        static auto getId() -> u32 { return ComponentTypeId::get<T>(); }

        explicit ComponentBase(const Node &node): Component(node) {}

        auto getTypeId() -> u32 override { return getId(); }
    };
}
