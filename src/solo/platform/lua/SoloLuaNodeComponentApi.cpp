/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloLuaScriptComponent.h"
#include "SoloTransform.h"
#include "SoloMeshRenderer.h"
#include "SoloSkyboxRenderer.h"
#include "SoloFontRenderer.h"
#include "SoloCamera.h"
#include "SoloSpectator.h"
#include "SoloLuaCommon.h"
#include "SoloRigidBody.h"
#include "SoloRenderContext.h"

using namespace solo;


static auto findComponent(Node *node, const std::string &name) -> Component*
{
    if (name == "Transform")
        return node->findComponent<Transform>();
    if (name == "MeshRenderer")
        return node->findComponent<MeshRenderer>();
    if (name == "Camera")
        return node->findComponent<Camera>();
    if (name == "Spectator")
        return node->findComponent<Spectator>();
    if (name == "SkyboxRenderer")
        return node->findComponent<SkyboxRenderer>();
    if (name == "FontRenderer")
        return node->findComponent<FontRenderer>();
    if (name == "RigidBody")
        return node->findComponent<RigidBody>();

    SL_PANIC("Unknown standard component ", name)
    return nullptr;
}


static auto addComponent(Node *node, const std::string &name, LuaRef arg) -> Component*
{
    if (name == "Transform")
        return node->addComponent<Transform>();
    if (name == "MeshRenderer")
        return node->addComponent<MeshRenderer>();
    if (name == "Camera")
        return node->addComponent<Camera>();
    if (name == "Spectator")
        return node->addComponent<Spectator>();
    if (name == "SkyboxRenderer")
        return node->addComponent<SkyboxRenderer>();
    if (name == "FontRenderer")
        return node->addComponent<FontRenderer>();
    if (name == "RigidBody")
        return node->addComponent<RigidBody>(arg.toValue<RigidBodyConstructionParameters>());

    SL_PANIC("Unknown standard component ", name)
    return nullptr;
}


static void removeComponent(Node *node, const std::string &name)
{
    if (name == "Transform")
        node->removeComponent<Transform>();
    else if (name == "MeshRenderer")
        node->removeComponent<MeshRenderer>();
    else if (name == "Camera")
        node->removeComponent<Camera>();
    else if (name == "Spectator")
        node->removeComponent<Spectator>();
    else if (name == "SkyboxRenderer")
        node->removeComponent<SkyboxRenderer>();
    else if (name == "FontRenderer")
        node->removeComponent<FontRenderer>();
    else if (name == "RigidBody")
        node->removeComponent<RigidBody>();
    else
        SL_PANIC("Unknown standard component ", name)
}


static auto findScriptComponent(Node *node, uint32_t typeId) -> LuaRef
{
    auto cmp = node->getScene()->findComponent(node->getId(), typeId + lua::ScriptComponent::MinComponentTypeId);
    if (cmp)
    {
        auto scriptComponent = dynamic_cast<lua::ScriptComponent*>(cmp);
        return scriptComponent->getRef();
    }

    return {};
}


static void addScriptComponent(Node *node, LuaRef scriptComponent)
{
    auto actualComponent = std::make_shared<lua::ScriptComponent>(*node, scriptComponent);
    node->getScene()->addComponent(node->getId(), actualComponent);
}


static void removeScriptComponent(Node *node, LuaRef scriptComponent)
{
    auto typeId = scriptComponent.get<uint32_t>("typeId") + lua::ScriptComponent::MinComponentTypeId;
    node->getScene()->removeComponent(node->getId(), typeId);
}


static void registerComponent(CppBindModule<LuaBinding> &module)
{
    auto component = BEGIN_CLASS(module, Component);
    REG_METHOD(component, Component, update);
    REG_METHOD(component, Component, render);
    REG_METHOD(component, Component, getTypeId);
    REG_METHOD(component, Component, getTag);
    REG_METHOD(component, Component, setTag);
    REG_METHOD(component, Component, getNode);
    component.endClass();
}


static void registerNode(CppBindModule<LuaBinding> &module)
{
    auto node = BEGIN_CLASS(module, Node);
    REG_METHOD(node, Node, getId);
    REG_METHOD(node, Node, getScene);
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
