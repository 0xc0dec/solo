#pragma once

#include "SoloScript.h"

namespace solo
{
	template <class TNode>
	class Node_ScriptWrap
	{
	public:
		virtual ~Node_ScriptWrap() {}

		Script* addScript();
		void removeScript();
	};
}