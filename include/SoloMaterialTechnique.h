#pragma once

#include "SoloBase.h"

namespace solo
{
	class MaterialPass;

	// Encapsulates one or multiple passes
	class MaterialTechnique
	{
	public:
		~MaterialTechnique() {}

		static ptr<MaterialTechnique> create();

		size_t getPassCount() const;
		ptr<MaterialPass> getPass(unsigned index) const;

	private:
		MaterialTechnique() {}

		std::vector<ptr<MaterialPass>> _passes;
	};
}