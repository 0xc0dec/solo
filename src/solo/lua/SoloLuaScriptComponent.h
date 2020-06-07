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
    // TODO register in Lua
    class LuaScriptComponent final : public ComponentBase<LuaScriptComponent>
    {
    public:
        static const u32 MinTypeId = 1000000000; // Assume that built-in components don't ever exceed this limit

        LuaScriptComponent(const Node& node, LuaRef scriptComponent);

        void init() override;
        void terminate() override;
        void update() override;
        void render() override;

        auto typeId() -> u32 override { return typeId_; }

        auto ref() const -> LuaRef { return ref_; }

    private:
        u32 typeId_;
        LuaRef ref_;

        std::function<void(LuaRef)> initFunc_;
        std::function<void(LuaRef)> terminateFunc_;
        std::function<void(LuaRef)> updateFunc_;
        std::function<void(LuaRef)> renderFunc_;
    };
}