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
    
    initFunc_ = scriptComponent.has("init")
        ? scriptComponent.get<std::function<void(LuaRef)>>("init")
        : [](LuaRef) {};
    
    updateFunc_ = scriptComponent.has("update")
        ? scriptComponent.get<std::function<void(LuaRef)>>("update")
        : [](LuaRef) {};

    renderFunc_ = scriptComponent.has("render")
        ? scriptComponent.get<std::function<void(LuaRef)>>("render")
        : [](LuaRef) {};
    
    terminateFunc_ = scriptComponent.has("terminate")
        ? scriptComponent.get<std::function<void(LuaRef)>>("terminate")
        : [](LuaRef) {};
    
    scriptComponent.set("node", node);
}

void LuaScriptComponent::init()
{
    initFunc_(ref_);
}

void LuaScriptComponent::terminate()
{
    terminateFunc_(ref_);
}

void LuaScriptComponent::update()
{
    updateFunc_(ref_);
}

void LuaScriptComponent::render()
{
    renderFunc_(ref_);
}
