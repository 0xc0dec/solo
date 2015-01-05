#pragma once

#include "SoloBaseInternal.h"

namespace solo
{
	class Pass;

	class Technique
	{
	public:

	private:
		std::list<ptr<Pass>> _passes;
	};
}