#pragma once

#include "scripting/SoloScripter.h"
#include <lua/lua.hpp>
#include <oolua.h>

namespace solo
{
	class LuaScripter: public Scripter
	{
	public:
		virtual void execString(const std::string& script) override;
		virtual void execFile(const std::string& scriptFileName) override;

		lua_State *getLuaState() const;

	private:
		friend class ScripterFactory;

		LuaScripter();
		void registerScriptApi();

		OOLUA::Script* script;
		lua_State *lua;
	};
}