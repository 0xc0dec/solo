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


lua::ScriptComponent::ScriptComponent(const Node &node, LuaRef scriptComponent):
    ComponentBase<ScriptComponent>(node),
    scriptComponent(scriptComponent)
{
    typeId = MinComponentTypeId + scriptComponent.get<uint32_t>("typeId");
    
    initFunc = scriptComponent.has("init")
        ? scriptComponent.get<std::function<void(LuaRef)>>("init")
        : [](LuaRef) {};
    
    updateFunc = scriptComponent.has("update")
        ? scriptComponent.get<std::function<void(LuaRef)>>("update")
        : [](LuaRef) {};
    
    terminateFunc = scriptComponent.has("terminate")
        ? scriptComponent.get<std::function<void(LuaRef)>>("terminate")
        : [](LuaRef) {};
    
    renderFunc = scriptComponent.has("render")
        ? scriptComponent.get<std::function<void(LuaRef, const RenderContext&)>>("render")
        : [](LuaRef, const RenderContext&) {};
    
    onComponentAddedFunc = scriptComponent.has("onComponentAddedFunc")
        ? scriptComponent.get<std::function<void(LuaRef, Component*)>>("onComponentAddedFunc")
        : [](LuaRef, Component*) {};
    
    onComponentRemovedFunc = scriptComponent.has("onComponentRemovedFunc")
        ? scriptComponent.get<std::function<void(LuaRef, Component*)>>("onComponentRemovedFunc")
        : [](LuaRef, Component*) {};
    
    scriptComponent.set("node", node);
}


void lua::ScriptComponent::init()
{
    initFunc(scriptComponent);
}


void lua::ScriptComponent::terminate()
{
    terminateFunc(scriptComponent);
}


void lua::ScriptComponent::update()
{
    updateFunc(scriptComponent);
}


void lua::ScriptComponent::render(const RenderContext &context)
{
    renderFunc(scriptComponent, context);
}


void lua::ScriptComponent::onComponentAdded(Component* cmp)
{
    onComponentAddedFunc(scriptComponent, cmp);
}


void lua::ScriptComponent::onComponentRemoved(Component* cmp)
{
    onComponentRemovedFunc(scriptComponent, cmp);
}
