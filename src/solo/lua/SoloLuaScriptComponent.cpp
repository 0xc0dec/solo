/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloLuaScriptComponent.h"
#include "SoloTransform.h"
#include "SoloLuaCommon.h"

using namespace solo;

LuaScriptComponent::LuaScriptComponent(const Node &node, LuaRef ref):
    ComponentBase<LuaScriptComponent>(node),
    ref_(ref) {
    typeId_ = sanitizeTypeId(ref.get<u32>("typeId"));

    initFunc_ = ref.has("init")
                ? ref.get<std::function<void(LuaRef)>>("init")
    : [](const LuaRef &) {};

    updateFunc_ = ref.has("update")
                  ? ref.get<std::function<void(LuaRef)>>("update")
    : [](const LuaRef &) {};

    renderFunc_ = ref.has("render")
                  ? ref.get<std::function<void(LuaRef)>>("render")
    : [](const LuaRef &) {};

    terminateFunc_ = ref.has("terminate")
                     ? ref.get<std::function<void(LuaRef)>>("terminate")
    : [](const LuaRef &) {};

    ref.set("node", node);
}

void LuaScriptComponent::init() {
    initFunc_(ref_);
}

void LuaScriptComponent::terminate() {
    terminateFunc_(ref_);
}

void LuaScriptComponent::update() {
    updateFunc_(ref_);
}

void LuaScriptComponent::render() {
    renderFunc_(ref_);
}
