/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
            static const auto id = counter_++;
            return id;
        }

    private:
        static u32 counter_;
    };

    class Component: public NoCopyAndMove
    {
    public:
        virtual ~Component() = default;

        virtual auto typeId() -> u32 = 0;

        virtual void init() {}
        virtual void terminate() {}
        
        virtual void update() {}
        virtual void render() {}

        auto node() const -> Node { return node_; }

        auto tag() const -> u32 { return tag_; }
        void setTag(u32 tag) { this->tag_ = tag; }

    protected:
        Node node_;
        u32 tag_ = 1;

        explicit Component(const Node &node): node_(node)
        {
        }
    };

    template <class T>
    class ComponentBase: public Component
    {
    public:
        static auto getId() -> u32 { return ComponentTypeId::get<T>(); }

        explicit ComponentBase(const Node &node): Component(node) {}

        auto typeId() -> u32 override { return getId(); }
    };
}
