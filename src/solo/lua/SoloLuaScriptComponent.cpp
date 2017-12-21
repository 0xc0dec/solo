/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloLuaScriptComponent.h"
#include "SoloTransform.h"
#include "SoloLuaCommon.h"

using namespace solo;

LuaScriptComponent::LuaScriptComponent(const Node &node, LuaRef ref):
    ComponentBase<LuaScriptComponent>(node),
    ref(ref)
{
    typeId = MinComponentTypeId + ref.get<u32>("typeId");
    
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
        ? ref.get<std::function<void(LuaRef)>>("render")
        : [](LuaRef) {};
    
    onComponentAddedFunc = ref.has("onComponentAddedFunc")
        ? ref.get<std::function<void(LuaRef, Component*)>>("onComponentAddedFunc")
        : [](LuaRef, Component*) {};
    
    onComponentRemovedFunc = ref.has("onComponentRemovedFunc")
        ? ref.get<std::function<void(LuaRef, Component*)>>("onComponentRemovedFunc")
        : [](LuaRef, Component*) {};
    
    ref.set("node", node);
}

void LuaScriptComponent::init()
{
    initFunc(ref);
}

void LuaScriptComponent::terminate()
{
    terminateFunc(ref);
}

void LuaScriptComponent::update()
{
    updateFunc(ref);
}

void LuaScriptComponent::render()
{
    renderFunc(ref);
}

void LuaScriptComponent::handleComponentAdded(Component* cmp)
{
    onComponentAddedFunc(ref, cmp);
}

void LuaScriptComponent::handleComponentRemoved(Component* cmp)
{
    onComponentRemovedFunc(ref, cmp);
}
