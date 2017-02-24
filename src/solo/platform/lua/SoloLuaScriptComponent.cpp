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

#include "SoloLuaScriptComponent.h"
#include "SoloRenderContext.h"
#include "SoloTransform.h"
#include "SoloLuaCommon.h"

using namespace solo;


lua::ScriptComponent::ScriptComponent(const Node &node, LuaRef ref):
    ComponentBase<ScriptComponent>(node),
    ref(ref)
{
    typeId = MinComponentTypeId + ref.get<uint32_t>("typeId");
    
    initFunc = ref.has("init")
        ? ref.get<std::function<void(LuaRef)>>("init")
        : [](LuaRef) {};
    
    updateFunc = ref.has("update")
        ? ref.get<std::function<void(LuaRef)>>("update")
        : [](LuaRef) {};
    
    terminateFunc = ref.has("terminate")
        ? ref.get<std::function<void(LuaRef)>>("terminate")
        : [](LuaRef) {};
    
    renderFunc = ref.has("render")
        ? ref.get<std::function<void(LuaRef, const RenderContext&)>>("render")
        : [](LuaRef, const RenderContext&) {};
    
    onComponentAddedFunc = ref.has("onComponentAddedFunc")
        ? ref.get<std::function<void(LuaRef, Component*)>>("onComponentAddedFunc")
        : [](LuaRef, Component*) {};
    
    onComponentRemovedFunc = ref.has("onComponentRemovedFunc")
        ? ref.get<std::function<void(LuaRef, Component*)>>("onComponentRemovedFunc")
        : [](LuaRef, Component*) {};
    
    ref.set("node", node);
}


void lua::ScriptComponent::init()
{
    initFunc(ref);
}


void lua::ScriptComponent::terminate()
{
    terminateFunc(ref);
}


void lua::ScriptComponent::update()
{
    updateFunc(ref);
}


void lua::ScriptComponent::render(const RenderContext &context)
{
    renderFunc(ref, context);
}


void lua::ScriptComponent::onComponentAdded(Component* cmp)
{
    onComponentAddedFunc(ref, cmp);
}


void lua::ScriptComponent::onComponentRemoved(Component* cmp)
{
    onComponentRemovedFunc(ref, cmp);
}
