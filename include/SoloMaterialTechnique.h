#pragma once

#include "SoloBase.h"

namespace solo
{
	class Pass;

	// Encapsulates one or multiple passes
	class MaterialTechnique
	{
	public:
		~MaterialTechnique() {}

		static ptr<MaterialTechnique> create();

		size_t getPassCount() const;
		ptr<Pass> getPass(unsigned index) const;

	private:
		MaterialTechnique() {}

		std::vector<ptr<Pass>> _passes;
	};
}