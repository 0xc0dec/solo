#pragma once

#include "SoloBase.h"

namespace solo
{
	class Engine;

	class ScriptManager
	{
	public:
		virtual ~ScriptManager() {}

		virtual void execute(const std::string& code) = 0;
		virtual void executeFile(const std::string& path) = 0;
	};

	class ScriptManagerFactory
	{
		friend class Engine;
		static shared<ScriptManager> create(Engine *engine);
	};
}
