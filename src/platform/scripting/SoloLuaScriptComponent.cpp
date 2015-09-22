#include "SoloLuaScriptComponent.h"

using namespace solo;
using namespace LuaIntf;

LuaScriptComponent::LuaScriptComponent(const Node& node, size_t typeId,
	std::function<void()> initFunc,
	std::function<void()> updateFunc,
	std::function<void()> renderFunc,
	std::function<void()> postRenderFunc,
	std::function<void()> terminateFunc) :
	ComponentBase<LuaScriptComponent>(node),
	typeId(typeId),
	initFunc(initFunc),
	updateFunc(updateFunc),
	renderFunc(renderFunc),
	postRenderFunc(postRenderFunc),
	terminateFunc(terminateFunc)
{
}


void LuaScriptComponent::init()
{
	initFunc();
}


void LuaScriptComponent::update()
{
	updateFunc();
}


void LuaScriptComponent::render(RenderContext& context)
{
	renderFunc(/* TODO pass context */);
}


void LuaScriptComponent::postRender()
{
	postRenderFunc();
}


void LuaScriptComponent::terminate()
{
	terminateFunc();
}


size_t LuaScriptComponent::getTypeId()
{
	return typeId;
}


void LuaScriptComponent::addComponent(LuaState& lua, Node* node, LuaRef& factoryFunction)
{
	auto factory = factoryFunction.toValue<std::function<LuaRef()>>();
	auto component = factory();
	auto typeIdString = component.get<std::string>("typeId");
	auto typeId = getHash(typeIdString);
	auto initFunc = component.get<std::function<void()>>("init");
	auto updateFunc = component.get<std::function<void()>>("update");
	auto renderFunc = component.get<std::function<void()>>("render");
	auto postRenderFunc = component.get<std::function<void()>>("postRender");
	auto terminateFunc = component.get<std::function<void()>>("terminate");
	node->addComponent<LuaScriptComponent>(typeId, initFunc, updateFunc, renderFunc, postRenderFunc, terminateFunc);
}


void LuaScriptComponent::removeComponent(Node* node, const std::string& componentTypeId)
{
	auto typeId = getHash(componentTypeId);
	node->getScene()->removeComponent(node->getId(), typeId);
}


std::function<void(Node*, LuaIntf::LuaRef&)> LuaScriptComponent::getAddComponentFunc(LuaState& lua)
{
	return std::function<void(Node*, LuaRef&)>(std::bind(&LuaScriptComponent::addComponent, lua, std::placeholders::_1, std::placeholders::_2));
}