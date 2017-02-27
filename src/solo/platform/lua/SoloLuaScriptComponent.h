/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloComponent.h"
#include <LuaIntf.h>
#include <functional>

namespace solo
{
    namespace lua
    {
        class ScriptComponent final : public ComponentBase<ScriptComponent>
        {
        public:
            static const uint32_t MinComponentTypeId = 1000000000; // Assume that built-in components don't ever exceed this limit

            ScriptComponent(const Node& node, LuaIntf::LuaRef scriptComponent);

            void init() override final;
            void terminate() override final;
            void update() override final;
            void render() override final;

            void onComponentAdded(Component *cmp) override final;
            void onComponentRemoved(Component *cmp) override final;

            auto getTypeId() -> uint32_t override final;

            auto getRef() const -> LuaIntf::LuaRef;

        private:
            uint32_t typeId;
            LuaIntf::LuaRef ref;

            std::function<void(LuaIntf::LuaRef)> initFunc;
            std::function<void(LuaIntf::LuaRef)> terminateFunc;
            std::function<void(LuaIntf::LuaRef)> updateFunc;
            std::function<void(LuaIntf::LuaRef)> renderFunc;
            std::function<void(LuaIntf::LuaRef, Component*)> onComponentAddedFunc;
            std::function<void(LuaIntf::LuaRef, Component*)> onComponentRemovedFunc;
        };

        inline auto ScriptComponent::getTypeId() -> uint32_t
        {
            return typeId;
        }

        inline auto ScriptComponent::getRef() const -> LuaIntf::LuaRef
        {
            return ref;
        }
    }
}