/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloComponent.h"
#include "SoloLuaCommon.h"
#include <functional>

namespace solo
{
    class LuaScriptComponent final : public ComponentBase<LuaScriptComponent>
    {
    public:
        static const u32 MinTypeId = 1000000000; // Assume that built-in components don't ever exceed this limit

        LuaScriptComponent(const Node& node, LuaIntf::LuaRef scriptComponent);

        void init() override final;
        void terminate() override final;
        void update() override final;
        void render() override final;

        auto getTypeId() -> u32 override final { return typeId; }

        auto getRef() const -> LuaIntf::LuaRef { return ref; }

    private:
        u32 typeId;
        LuaIntf::LuaRef ref;

        std::function<void(LuaIntf::LuaRef)> initFunc;
        std::function<void(LuaIntf::LuaRef)> terminateFunc;
        std::function<void(LuaIntf::LuaRef)> updateFunc;
        std::function<void(LuaIntf::LuaRef)> renderFunc;
    };
}