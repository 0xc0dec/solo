#include "SoloLuaScriptComponent.h"
#include "SoloTransform.h"
#include "SoloCamera.h"
#include "SoloMeshRenderer.h"
#include "SoloSpectator.h"
#include "SoloSkyboxRenderer.h"
#include "SoloRenderContext.h"

using namespace solo;
using namespace LuaIntf;


int LuaScriptComponent::counter = 1000000000;
std::unordered_map<std::string, int> LuaScriptComponent::typeIds;


LuaScriptComponent::LuaScriptComponent(const Node& node, LuaRef& component) :
    ComponentBase<LuaScriptComponent>(node),
    component(component)
{
    auto name = component.get<std::string>("name");
    typeId = getOrRegisterComponentTypeId(name);
    initFunc = component.has("init") ? component.get<std::function<void(LuaRef)>>("init") : [](LuaRef) {};
    updateFunc = component.has("update") ? component.get<std::function<void(LuaRef)>>("update") : [](LuaRef) {};
    terminateFunc = component.has("terminate") ? component.get<std::function<void(LuaRef)>>("terminate") : [](LuaRef) {};
    renderFunc = component.has("render") ? component.get<std::function<void(LuaRef, RenderContext&)>>("render") : [](LuaRef, RenderContext&) {};
    onAfterCameraRenderFunc = component.has("onAfterCameraRender") ? component.get<std::function<void(LuaRef)>>("onAfterCameraRender") : [](LuaRef) {};
    component.set("node", node);
}


void LuaScriptComponent::init()
{
    initFunc(component);
}


void LuaScriptComponent::update()
{
    updateFunc(component);
}


void LuaScriptComponent::terminate()
{
    terminateFunc(component);
}


void LuaScriptComponent::render(RenderContext& context)
{
    renderFunc(component, context);
}


void LuaScriptComponent::onAfterCameraRender()
{
    onAfterCameraRenderFunc(component);
}


int LuaScriptComponent::getTypeId()
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


std::function<LuaRef(Node*, const std::string&)> LuaScriptComponent::getFindScriptFunc(lua_State* lua)
{
    return std::bind(&findScript, lua, std::placeholders::_1, std::placeholders::_2);
}


LuaRef LuaScriptComponent::findScript(lua_State* lua, Node* node, const std::string& componentTypeId)
{
    auto typeId = findComponentTypeId(componentTypeId);
    if (typeId < 0) // TODO not very cool
        return LuaRef(lua, nullptr);

    auto component = node->getScene()->findComponent(node->getId(), typeId);
    auto scriptComponent = dynamic_cast<LuaScriptComponent*>(component);
    return scriptComponent ? scriptComponent->component : LuaRef(lua, nullptr);
}


int LuaScriptComponent::getOrRegisterComponentTypeId(const std::string& typeName)
{
    auto typeId = findComponentTypeId(typeName);
    if (typeId < 0) // TODO not very cool
    {
        typeId = counter++;
        typeIds[typeName] = typeId;
    }
    return typeId;
}


int LuaScriptComponent::findComponentTypeId(const std::string& typeName)
{
    auto it = typeIds.find(typeName);
    return it != typeIds.end() ? it->second : -1;
}


void LuaScriptComponent::addScript(Node* node, LuaRef& component)
{
    auto actualComponent = SL_MAKE_SHARED<LuaScriptComponent>(*node, component);
    node->getScene()->addComponent(node->getId(), actualComponent);
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
    SL_THROW_FMT(EngineException, "Unknown standard component ", typeName);
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


void LuaScriptComponent::removeScript(Node* node, const std::string& componentTypeId)
{
    auto typeId = findComponentTypeId(componentTypeId);
    if (typeId >= 0) // TODO not very cool
        node->getScene()->removeComponent(node->getId(), typeId);
}
