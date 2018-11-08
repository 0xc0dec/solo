/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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

        LuaScriptComponent(const Node& node, LuaRef scriptComponent);

        void init() override final;
        void terminate() override final;
        void update() override final;
        void render() override final;

        auto typeId() -> u32 override final { return typeId_; }

        auto ref() const -> LuaRef { return ref_; }

    private:
        u32 typeId_;
        LuaRef ref_;

        std::function<void(LuaRef)> initFunc;
        std::function<void(LuaRef)> terminateFunc;
        std::function<void(LuaRef)> updateFunc;
        std::function<void(LuaRef)> renderFunc;
    };
}