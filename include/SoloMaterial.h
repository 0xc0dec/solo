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

		void addTechnique(ptr<Technique> technique);
		void removeTechnique(ptr<Technique> technique);
		void setTechnique(ptr<Technique> technique);
		ptr<Technique> getTechnique(unsigned index) const;
		ptr<Technique> getCurrentTechnique() const;
		size_t getTechniquesCount() const;

	private:
		Material() {}

		ptr<Technique> _currentTechnique;
		std::vector<ptr<Technique>> _techniques;
	};
}
