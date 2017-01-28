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
#include "SoloMeshRenderer.h"
#include "SoloSkyboxRenderer.h"
#include "SoloFontRenderer.h"
#include "SoloCamera.h"
#include "SoloSpectator.h"
#include "SoloLuaCommon.h"

using namespace solo;


static const uint32_t MinComponentTypeId = 1000000000; // Assume that built-in components don't ever exceed this limit


LuaScriptComponent::LuaScriptComponent(const Node &node, LuaRef scriptComponent):
    ComponentBase<LuaScriptComponent>(node),
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


void LuaScriptComponent::init()
{
    initFunc(scriptComponent);
}


void LuaScriptComponent::terminate()
{
    terminateFunc(scriptComponent);
}


void LuaScriptComponent::update()
{
    updateFunc(scriptComponent);
}


void LuaScriptComponent::render(const RenderContext &context)
{
    renderFunc(scriptComponent, context);
}


void LuaScriptComponent::onComponentAdded(Component* cmp)
{
    onComponentAddedFunc(scriptComponent, cmp);
}


void LuaScriptComponent::onComponentRemoved(Component* cmp)
{
    onComponentRemovedFunc(scriptComponent, cmp);
}


static auto findComponent(Node* node, const std::string &name) -> Component*
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

    SL_ERR("Unknown standard component ", name)
    return nullptr;
}


static auto addComponent(Node *node, const std::string &name) -> Component*
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
        node->addComponent<FontRenderer>();

    SL_ERR("Unknown standard component ", name)
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
    else
        SL_ERR("Unknown standard component ", name)
}


static void addScriptComponent(Node *node, LuaRef scriptComponent)
{
    auto actualComponent = std::make_shared<LuaScriptComponent>(*node, scriptComponent);
    node->getScene()->addComponent(node->getId(), actualComponent);
}


static void removeScriptComponent(Node* node, LuaRef scriptComponent)
{
    auto typeId = scriptComponent.get<uint32_t>("typeId") + MinComponentTypeId;
    node->getScene()->removeComponent(node->getId(), typeId);
}


void registerNodeAndComponent(CppBindModule<LuaBinding> &module)
{
    auto component = module.beginClass<Component>("Component");
    REGISTER_METHOD(component, Component, getTypeId);
    REGISTER_METHOD(component, Component, getTags);
    REGISTER_METHOD(component, Component, setTags);
    REGISTER_METHOD(component, Component, getNode);
    component.endClass();

    auto node = module.beginClass<Node>("Node");
    REGISTER_METHOD(node, Node, getId);
    REGISTER_METHOD(node, Node, getScene);
    node.addFunction("addScriptComponent", addScriptComponent);
    node.addFunction("removeScriptComponent", removeScriptComponent);
    node.addFunction("findComponent", findComponent);
    node.addFunction("addComponent", addComponent);
    node.addFunction("removeComponent", removeComponent);
    node.endClass();
}
