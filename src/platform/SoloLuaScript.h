#pragma once

#include "SoloScript.h"
#include <lua.hpp>
#include <LuaBridge.h>

namespace solo
{
	class LuaScript : public Script
	{
	public:
		~LuaScript() {}

		virtual void update() override;

	private:
		friend class ScriptFactory;

		LuaScript(Node* node, const std::string& componentObjectName);

		luabridge::LuaRef callback;
	};
}