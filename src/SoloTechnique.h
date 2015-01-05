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

		virtual unsigned getPassCount() const;
		virtual ptr<Pass> getPass(unsigned index) const;

	private:
		Technique() {}
		virtual ~Technique() {}

		std::vector<ptr<Pass>> _passes;
	};
}