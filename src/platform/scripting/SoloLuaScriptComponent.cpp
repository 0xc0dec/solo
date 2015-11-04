#include "SoloLuaScriptComponent.h"
#include "SoloTransform.h"
#include "SoloCamera.h"
#include "SoloModelRenderer.h"
#include "SoloSpectator.h"

using namespace solo;
using namespace LuaIntf;


LuaScriptComponent::LuaScriptComponent(const Node& node, LuaRef& component) :
	ComponentBase<LuaScriptComponent>(node),
	component(component)
{
	auto typeIdString = component.get<std::string>("typeId");
	typeId = getHash(typeIdString);
	initFunc = component.has("init") ? component.get<std::function<void(LuaRef)>>("init") : [](LuaRef) {};
	updateFunc = component.has("update") ? component.get<std::function<void(LuaRef)>>("update") : [](LuaRef) {};
	terminateFunc = component.has("terminate") ? component.get<std::function<void(LuaRef)>>("terminate") : [](LuaRef) {};
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


void LuaScriptComponent::onAfterCameraRender()
{
	onAfterCameraRenderFunc(component);
}


size_t LuaScriptComponent::getTypeId()
{
	return typeId;
}


Component* LuaScriptComponent::findComponent(Node* node, const std::string& typeName)
{
	if (typeName == "Transform")
		return node->findComponent<Transform>();
	if (typeName == "ModelRenderer")
		return node->findComponent<ModelRenderer>();
	if (typeName == "Camera")
		return node->findComponent<Camera>();
	if (typeName == "Spectator")
		return node->findComponent<Spectator>();
	return nullptr;
}


std::function<LuaRef(Node*, const std::string&)> LuaScriptComponent::getFindScriptComponentFunc(lua_State* lua)
{
	return std::bind(&findScriptComponent, lua, std::placeholders::_1, std::placeholders::_2);
}


LuaRef LuaScriptComponent::findScriptComponent(lua_State *lua, Node* node, const std::string& componentTypeId)
{
	auto typeId = getHash(componentTypeId);
	auto component = node->getScene()->findComponent(node->getId(), typeId);
	auto scriptComponent = dynamic_cast<LuaScriptComponent*>(component);
	return scriptComponent ? scriptComponent->component : LuaRef(lua, nullptr);
}


void LuaScriptComponent::addScriptComponent(Node* node, LuaRef& component)
{
	auto actualComponent = SL_NEW<LuaScriptComponent>(*node, component);
	node->getScene()->addComponent(node->getId(), actualComponent);
}


Component* LuaScriptComponent::addComponent(Node* node, const std::string& typeName)
{
	if (typeName == "Transform")
		return node->addComponent<Transform>();
	if (typeName == "ModelRenderer")
		return node->addComponent<ModelRenderer>();
	if (typeName == "Camera")
		return node->addComponent<Camera>();
	if (typeName == "Spectator")
		return node->addComponent<Spectator>();
	SL_THROW_FMT(EngineException, "Unknown standard component ", typeName);
}


void LuaScriptComponent::removeComponent(Node* node, const std::string& typeName)
{
	if (typeName == "Transform")
		node->removeComponent<Transform>();
	if (typeName == "ModelRenderer")
		node->removeComponent<ModelRenderer>();
	if (typeName == "Camera")
		node->removeComponent<Camera>();
	if (typeName == "Spectator")
		node->removeComponent<Spectator>();
}


void LuaScriptComponent::removeScriptComponent(Node* node, const std::string& componentTypeId)
{
	auto typeId = getHash(componentTypeId);
	node->getScene()->removeComponent(node->getId(), typeId);
}
