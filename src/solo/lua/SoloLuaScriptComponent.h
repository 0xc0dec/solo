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
		static auto sanitizeTypeId(u32 typeId) -> u32 { return TYPE_ID_BASE + typeId; }
    	
        LuaScriptComponent(const Node& node, LuaRef ref);

        void init() override;
        void terminate() override;
        void update() override;
        void render() override;

        auto typeId() -> u32 override { return typeId_; }

        auto ref() const -> LuaRef { return ref_; }

    private:
    	static const u32 TYPE_ID_BASE = 1000000000; // Assume that built-in components don't ever exceed this limit
    	
        u32 typeId_;
        LuaRef ref_;

        std::function<void(LuaRef)> initFunc_;
        std::function<void(LuaRef)> terminateFunc_;
        std::function<void(LuaRef)> updateFunc_;
        std::function<void(LuaRef)> renderFunc_;
    };
}