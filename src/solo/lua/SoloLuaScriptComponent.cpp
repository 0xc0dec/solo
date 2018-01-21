/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloLuaScriptComponent.h"
#include "SoloTransform.h"
#include "SoloLuaCommon.h"

using namespace solo;

LuaScriptComponent::LuaScriptComponent(const Node &node, sol::table &cmp):
    ComponentBase<LuaScriptComponent>(node),
	cmp(cmp)
{
    typeId = minComponentTypeId + cmp.get<u32>("typeId");
    
    initFunc = cmp["init"].valid()
        ? cmp.get<std::function<void(sol::object)>>("init")
        : [](sol::object) {};
    
    updateFunc = cmp["update"].valid()
        ? cmp.get<std::function<void(sol::object)>>("update")
        : [](sol::object) {};
    
    terminateFunc = cmp["terminate"].valid()
        ? cmp.get<std::function<void(sol::object)>>("terminate")
        : [](sol::object) {};
    
    cmp.set("node", node);
}

void LuaScriptComponent::init()
{
    initFunc(cmp);
}

void LuaScriptComponent::terminate()
{
    terminateFunc(cmp);
}

void LuaScriptComponent::update()
{
    updateFunc(cmp);
}
