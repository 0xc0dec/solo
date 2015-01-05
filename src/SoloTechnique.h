#pragma once

#include "SoloBaseInternal.h"

namespace solo
{
	class Pass;

	// Encapsulates one or multiple passes
	class Technique
	{
	public:
		static ptr<Technique> create();

	private:
		Technique() {}
		virtual ~Technique() {}

		std::vector<ptr<Pass>> _passes;
	};
}