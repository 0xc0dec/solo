#pragma once

#include "SoloBase.h"
#include "SoloScript.h"

namespace solo
{
	template <class TNode>
	class ScriptedNode
	{
	public:
		virtual ~ScriptedNode() {}

		void addScriptComponent(const std::string& componentObjectName);
	};
}