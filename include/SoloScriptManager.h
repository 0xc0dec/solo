#pragma once

#include "SoloBase.h"

namespace solo
{
	class ScriptManager
	{
	public:
		virtual ~ScriptManager() {}

		static shared<ScriptManager> create();

		virtual void execute(const std::string& code) = 0;
		virtual void executeFile(const std::string& path) = 0;
	};
}
