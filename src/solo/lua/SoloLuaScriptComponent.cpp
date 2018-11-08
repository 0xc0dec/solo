/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloLuaScriptComponent.h"
#include "SoloTransform.h"
#include "SoloLuaCommon.h"

using namespace solo;

LuaScriptComponent::LuaScriptComponent(const Node &node, LuaRef scriptComponent):
    ComponentBase<LuaScriptComponent>(node),
    ref_(scriptComponent)
{
    typeId_ = MinTypeId + scriptComponent.get<u32>("typeId");
    
    initFunc = scriptComponent.has("init")
        ? scriptComponent.get<std::function<void(LuaRef)>>("init")
        : [](LuaRef) {};
    
    updateFunc = scriptComponent.has("update")
        ? scriptComponent.get<std::function<void(LuaRef)>>("update")
        : [](LuaRef) {};

    renderFunc = scriptComponent.has("render")
        ? scriptComponent.get<std::function<void(LuaRef)>>("render")
        : [](LuaRef) {};
    
    terminateFunc = scriptComponent.has("terminate")
        ? scriptComponent.get<std::function<void(LuaRef)>>("terminate")
        : [](LuaRef) {};
    
    scriptComponent.set("node", node);
}

void LuaScriptComponent::init()
{
    initFunc(ref_);
}

void LuaScriptComponent::terminate()
{
    terminateFunc(ref_);
}

void LuaScriptComponent::update()
{
    updateFunc(ref_);
}

void LuaScriptComponent::render()
{
    renderFunc(ref_);
}
