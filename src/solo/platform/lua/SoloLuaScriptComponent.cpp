/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloLuaScriptComponent.h"
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


void lua::ScriptComponent::render()
{
    renderFunc(ref);
}


void lua::ScriptComponent::onComponentAdded(Component* cmp)
{
    onComponentAddedFunc(ref, cmp);
}


void lua::ScriptComponent::onComponentRemoved(Component* cmp)
{
    onComponentRemovedFunc(ref, cmp);
}
