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
		size_t getTechniquesCount() const;
		ptr<Technique> getTechnique(unsigned index) const;
		
		void setCurrentTechnique(ptr<Technique> technique);
		void setCurrentTechnique(unsigned index);
		ptr<Technique> getCurrentTechnique() const;

	private:
		Material() {}

		ptr<Technique> _currentTechnique;
		std::vector<ptr<Technique>> _techniques;
	};
}
