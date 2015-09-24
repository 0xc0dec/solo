#pragma once

#include <functional>
#include <LuaIntf.h>
#include "SoloComponent.h"


namespace solo
{
	class LuaScriptComponent: public ComponentBase<LuaScriptComponent>
	{
	public:
		LuaScriptComponent(
			const Node& node,
			size_t typeId,
			LuaIntf::LuaRef& component,
			std::function<void(LuaIntf::LuaRef)> initFunc,
			std::function<void(LuaIntf::LuaRef)> updateFunc,
			std::function<void(LuaIntf::LuaRef)> renderFunc,
			std::function<void(LuaIntf::LuaRef)> postRenderFunc,
			std::function<void(LuaIntf::LuaRef)> terminateFunc);

		virtual void init() override;
		virtual void update() override;
		virtual void render(RenderContext& context) override;
		virtual void postRender() override;
		virtual void terminate() override;

		virtual size_t getTypeId() override;

		static Component* findStandardComponent(Node *node, const std::string& typeName);
		static std::function<LuaIntf::LuaRef(Node *, const std::string&)> getFindComponentFunc(lua_State *lua);
		static void addComponent(Node *node, LuaIntf::LuaRef& component);
		static Component* addStandardComponent(Node *node, const std::string& typeName);
		static void removeComponent(Node *node, const std::string& componentTypeId);

	private:
		static LuaIntf::LuaRef findComponent(lua_State *lua, Node *node, const std::string& componentTypeId);

		size_t typeId;
		LuaIntf::LuaRef component;
		std::function<void(LuaIntf::LuaRef component)> initFunc;
		std::function<void(LuaIntf::LuaRef)> updateFunc;
		std::function<void(LuaIntf::LuaRef)> renderFunc;
		std::function<void(LuaIntf::LuaRef)> postRenderFunc;
		std::function<void(LuaIntf::LuaRef)> terminateFunc;
	};
}