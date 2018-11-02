/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloLuaScriptComponent.h"
#include "SoloTransform.h"
#include "SoloMeshRenderer.h"
#include "SoloCamera.h"
#include "SoloSpectator.h"
#include "SoloLuaCommon.h"
#include "SoloRigidBody.h"

using namespace solo;

static umap<str, u32> builtInComponents = {
    {"Transform", Transform::getId()},
    {"MeshRenderer", MeshRenderer::getId()},
    {"Camera", Camera::getId()},
    {"Spectator", Spectator::getId()},
    {"RigidBody", RigidBody::getId()}
};

static auto findComponent(Node *node, const str &name) -> Component*
{
    SL_DEBUG_PANIC(!builtInComponents.count(name), "Not found built-in component ", name);
    return node->scene()->findComponent(node->id(), builtInComponents.at(name));
}

static auto addComponent(Node *node, const str &name, const LuaRef& arg) -> Component*
{
    if (name == "Transform")
        return node->addComponent<Transform>();
    if (name == "MeshRenderer")
        return node->addComponent<MeshRenderer>();
    if (name == "Camera")
        return node->addComponent<Camera>();
    if (name == "Spectator")
        return node->addComponent<Spectator>();
    if (name == "RigidBody")
        return node->addComponent<RigidBody>(arg.toValue<RigidBodyConstructionParameters>());

    SL_DEBUG_PANIC(true, "Unknown built-in component ", name);
    return nullptr;
}

static void removeComponent(Node *node, const str &name)
{
    SL_DEBUG_PANIC(!builtInComponents.count(name), "Not found built-in component ", name);
    node->scene()->removeComponent(node->id(), builtInComponents.at(name));
}

static auto findScriptComponent(Node *node, u32 typeId) -> LuaRef
{
    const auto cmp = node->scene()->findComponent(node->id(), typeId + LuaScriptComponent::MinTypeId);
    if (cmp)
    {
        const auto scriptComponent = dynamic_cast<LuaScriptComponent*>(cmp);
        return scriptComponent->getRef();
    }
    return {};
}

static auto addScriptComponent(Node *node, LuaRef scriptComponent) -> sptr<Component>
{
    const auto actualComponent = std::make_shared<LuaScriptComponent>(*node, scriptComponent);
    node->scene()->addComponent(node->id(), actualComponent);
    return actualComponent;
}

static void removeScriptComponent(Node *node, const LuaRef& scriptComponent)
{
    const auto typeId = scriptComponent.get<u32>("typeId") + LuaScriptComponent::MinTypeId;
    node->scene()->removeComponent(node->id(), typeId);
}

static void registerComponent(CppBindModule<LuaBinding> &module)
{
    auto component = BEGIN_CLASS(module, Component);
    REG_METHOD(component, Component, update);
    REG_METHOD(component, Component, render);
    REG_METHOD(component, Component, typeId);
    REG_METHOD(component, Component, tag);
    REG_METHOD(component, Component, setTag);
    REG_METHOD(component, Component, node);
    component.endClass();
}

static void registerNode(CppBindModule<LuaBinding> &module)
{
    auto node = BEGIN_CLASS(module, Node);
    REG_METHOD(node, Node, id);
    REG_METHOD(node, Node, scene);
    REG_FREE_FUNC_AS_METHOD(node, findScriptComponent);
    REG_FREE_FUNC_AS_METHOD(node, addScriptComponent);
    REG_FREE_FUNC_AS_METHOD(node, removeScriptComponent);
    REG_FREE_FUNC_AS_METHOD(node, findComponent);
    REG_FREE_FUNC_AS_METHOD(node, addComponent);
    REG_FREE_FUNC_AS_METHOD(node, removeComponent);
    node.endClass();
}

void registerNodeAndComponentApi(CppBindModule<LuaBinding> &module)
{
    registerComponent(module);
    registerNode(module);
}
