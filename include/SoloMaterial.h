#pragma once

#include "SoloBase.h"

namespace solo
{
	class Technique;

	class Material
	{
	public:
		~Material() {}

		static ptr<Material> create();

		unsigned getTechniquesCount() const;
		ptr<Technique> getTechnique(unsigned index) const;

	private:
		Material() {}

		ptr<Technique> _currentTechnique;
		std::vector<ptr<Technique>> _techniques;
	};
}
