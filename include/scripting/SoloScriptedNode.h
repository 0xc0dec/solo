#pragma once

#include "SoloScript.h"


namespace solo
{
	template <class TNode>
	class ScriptedNode
	{
	public:
		virtual ~ScriptedNode() {}

		void addScript(const std::string& callbackObjectName);
	};
}