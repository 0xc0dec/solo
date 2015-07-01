#pragma once

#include "SoloScripter.h"
#include <lua.hpp>

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

		lua_State *lua;
	};
}