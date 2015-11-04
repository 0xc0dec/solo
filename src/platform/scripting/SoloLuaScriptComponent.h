#pragma once

#include <functional>
#include <LuaIntf.h>
#include "SoloComponent.h"


namespace solo
{
	class LuaScriptComponent: public ComponentBase<LuaScriptComponent>
	{
	public:
		LuaScriptComponent(const Node& node, LuaIntf::LuaRef& component);

		virtual void init() override;
		virtual void update() override;
		virtual void terminate() override;
		virtual void onAfterCameraRender() override;

		virtual size_t getTypeId() override;

		static Component* findComponent(Node *node, const std::string& typeName);
		static Component* addComponent(Node *node, const std::string& typeName);
		static void removeComponent(Node *node, const std::string& typeName);

		static std::function<LuaIntf::LuaRef(Node *, const std::string&)> getFindScriptComponentFunc(lua_State *lua);
		static void addScriptComponent(Node *node, LuaIntf::LuaRef& component);
		static void removeScriptComponent(Node *node, const std::string& componentTypeId);

	private:
		static LuaIntf::LuaRef findScriptComponent(lua_State *lua, Node *node, const std::string& componentTypeId);

		size_t typeId;
		LuaIntf::LuaRef component;
		std::function<void(LuaIntf::LuaRef)> initFunc;
		std::function<void(LuaIntf::LuaRef)> updateFunc;
		std::function<void(LuaIntf::LuaRef)> terminateFunc;
		std::function<void(LuaIntf::LuaRef)> onAfterCameraRenderFunc;
	};
}