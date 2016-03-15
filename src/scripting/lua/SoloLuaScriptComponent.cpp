#include "SoloLuaScriptComponent.h"
#include "SoloTransform.h"
#include "SoloCamera.h"
#include "SoloMeshRenderer.h"
#include "SoloSpectator.h"
#include "SoloSkyboxRenderer.h"
#include "SoloRenderContext.h"

using namespace solo;
using namespace LuaIntf;

const uint32_t MinComponentTypeId = 1000000000; // it's expected that normal component ids should stop at this point


LuaScriptComponent::LuaScriptComponent(const Node& node, LuaRef scriptComponent) :
    ComponentBase<LuaScriptComponent>(node),
    scriptComponent(scriptComponent)
{
    typeId = scriptComponent.get<uint32_t>("typeId") + MinComponentTypeId;
    initFunc = scriptComponent.has("init") ? scriptComponent.get<std::function<void(LuaRef)>>("init") : [](LuaRef) {};
    updateFunc = scriptComponent.has("update") ? scriptComponent.get<std::function<void(LuaRef)>>("update") : [](LuaRef) {};
    terminateFunc = scriptComponent.has("terminate") ? scriptComponent.get<std::function<void(LuaRef)>>("terminate") : [](LuaRef) {};
    renderFunc = scriptComponent.has("render") ? scriptComponent.get<std::function<void(LuaRef, RenderContext&)>>("render") : [](LuaRef, RenderContext&) {};
    onAfterCameraRenderFunc = scriptComponent.has("onAfterCameraRender") ? scriptComponent.get<std::function<void(LuaRef)>>("onAfterCameraRender") : [](LuaRef) {};
    scriptComponent.set("node", node);
}


void LuaScriptComponent::init()
{
    initFunc(scriptComponent);
}


void LuaScriptComponent::update()
{
    updateFunc(scriptComponent);
}


void LuaScriptComponent::terminate()
{
    terminateFunc(scriptComponent);
}


void LuaScriptComponent::render(RenderContext& context)
{
    renderFunc(scriptComponent, context);
}


void LuaScriptComponent::onAfterCameraRender()
{
    onAfterCameraRenderFunc(scriptComponent);
}


uint32_t LuaScriptComponent::getTypeId()
{
    return typeId;
}


Component* LuaScriptComponent::findComponent(Node* node, const std::string& typeName)
{
    if (typeName == "Transform")
        return node->findComponent<Transform>();
    if (typeName == "MeshRenderer")
        return node->findComponent<MeshRenderer>();
    if (typeName == "Camera")
        return node->findComponent<Camera>();
    if (typeName == "Spectator")
        return node->findComponent<Spectator>();
    if (typeName == "SkyboxRenderer")
        return node->findComponent<SkyboxRenderer>();
    return nullptr;
}


void LuaScriptComponent::addScript(Node* node, LuaRef scriptComponent)
{
    auto actualComponent = SL_MAKE_SHARED<LuaScriptComponent>(*node, scriptComponent);
    node->getScene()->addComponent(node->getId(), actualComponent);
}


void LuaScriptComponent::removeScript(Node* node, LuaRef scriptComponent)
{
    auto typeId = scriptComponent.get<uint32_t>("typeId") + MinComponentTypeId;
    node->getScene()->removeComponent(node->getId(), typeId);
}


Component* LuaScriptComponent::addComponent(Node* node, const std::string& typeName)
{
    if (typeName == "Transform")
        return node->addComponent<Transform>();
    if (typeName == "MeshRenderer")
        return node->addComponent<MeshRenderer>();
    if (typeName == "Camera")
        return node->addComponent<Camera>();
    if (typeName == "Spectator")
        return node->addComponent<Spectator>();
    if (typeName == "SkyboxRenderer")
        return node->addComponent<SkyboxRenderer>();
    SL_FMT_THROW(InvalidInputException, "Unknown standard component ", typeName);
}


void LuaScriptComponent::removeComponent(Node* node, const std::string& typeName)
{
    if (typeName == "Transform")
        node->removeComponent<Transform>();
    if (typeName == "MeshRenderer")
        node->removeComponent<MeshRenderer>();
    if (typeName == "Camera")
        node->removeComponent<Camera>();
    if (typeName == "Spectator")
        node->removeComponent<Spectator>();
    if (typeName == "SkyboxRenderer")
        node->removeComponent<SkyboxRenderer>();
}
