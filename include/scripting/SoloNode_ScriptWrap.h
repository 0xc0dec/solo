#pragma once

#include "SoloScript.h"

namespace solo
{
	template <class TNode>
	class Node_ScriptWrap
	{
	public:
		virtual ~Node_ScriptWrap() {}

		void addScript(const std::string& callbackObjectName);
		void removeScript();
	};
}