#pragma once

#include "scripting/SoloScript.h"
#include <lua.hpp>


namespace solo
{
	class Device;

	class LuaScript : public Script
	{
	public:
		~LuaScript() {}

		virtual void update() override;

	private:
		friend class ScriptFactory;

		LuaScript(Node* node, const std::string& callbackObjectName);

		Device* device = nullptr;
		lua_State* lua = nullptr;
		std::string callbackObjectName;
	};
}