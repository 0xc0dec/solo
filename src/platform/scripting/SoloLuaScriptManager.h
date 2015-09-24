#pragma once

#include "SoloScriptManager.h"

#include <LuaIntf.h>

namespace solo
{
	class LuaScriptManager: public ScriptManager
	{
	public:
		virtual ~LuaScriptManager();

		virtual void execute(const std::string& code) override;
		virtual void executeFile(const std::string& path) override;

	private:
		friend class ScriptManager;

		LuaScriptManager();

		void registerApi();

		LuaIntf::LuaState lua;
	};
}