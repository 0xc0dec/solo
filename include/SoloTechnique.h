#pragma once

#include "SoloBase.h"

namespace solo
{
	class Pass;

	// Encapsulates one or multiple passes
	class Technique
	{
	public:
		~Technique() {}

		static ptr<Technique> create();

		unsigned getPassCount() const;
		ptr<Pass> getPass(unsigned index) const;

	private:
		Technique() {}

		std::vector<ptr<Pass>> _passes;
	};
}