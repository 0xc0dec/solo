#pragma once

#include "SoloScriptManager.h"

#define LUAINTF_LINK_LUA_COMPILED_IN_CXX 0
#include <LuaIntf.h>

namespace solo
{
	class Engine;

	class LuaScriptManager: public ScriptManager
	{
	public:
		virtual ~LuaScriptManager();

		virtual void execute(const std::string& code) override;
		virtual void executeFile(const std::string& path) override;

	private:
		friend class ScriptManagerFactory;

		LuaScriptManager(Engine *engine);

		void registerApi();

		Engine *engine;
		LuaIntf::LuaState lua;
	};
}