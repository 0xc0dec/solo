#pragma once

#include "SoloBaseInternal.h"
#include "SoloIMaterial.h"

namespace solo
{
	class Technique;

	class Material: public IMaterial
	{
	public:
		static ptr<Material> create();

		virtual unsigned getTechniquesCount() const;
		virtual ptr<Technique> getTechnique(unsigned index) const;

	private:
		Material() {}
		virtual ~Material() override {}

		ptr<Technique> _currentTechnique;
		std::vector<ptr<Technique>> _techniques;
	};
}
