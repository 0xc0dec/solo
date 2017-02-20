/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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
            void render(const RenderContext &context) override final;

            void onComponentAdded(Component *cmp) override final;
            void onComponentRemoved(Component *cmp) override final;

            auto getTypeId()->uint32_t override final;

            LuaIntf::LuaRef scriptComponent;

        private:
            uint32_t typeId;

            std::function<void(LuaIntf::LuaRef)> initFunc;
            std::function<void(LuaIntf::LuaRef)> terminateFunc;
            std::function<void(LuaIntf::LuaRef)> updateFunc;
            std::function<void(LuaIntf::LuaRef, const RenderContext&)> renderFunc;
            std::function<void(LuaIntf::LuaRef, Component*)> onComponentAddedFunc;
            std::function<void(LuaIntf::LuaRef, Component*)> onComponentRemovedFunc;
        };

        inline auto ScriptComponent::getTypeId() -> uint32_t
        {
            return typeId;
        }
    }
}