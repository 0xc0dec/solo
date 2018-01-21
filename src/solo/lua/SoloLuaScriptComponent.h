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
        static const u32 minComponentTypeId = 1000000000; // Assume that built-in components don't ever exceed this limit

        LuaScriptComponent(const Node& node, sol::table &cmp);

        void init() override final;
        void terminate() override final;
        void update() override final;

        auto getTypeId() -> u32 override final { return typeId; }
		auto getUnderlyingCmp() const -> sol::object { return cmp; }

    private:
        u32 typeId;
		sol::object cmp;

        std::function<void(sol::object)> initFunc;
        std::function<void(sol::object)> terminateFunc;
        std::function<void(sol::object)> updateFunc;
    };
}