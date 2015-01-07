#pragma once

#include "SoloBase.h"

namespace solo
{
	class MaterialTechnique;
	class MaterialParameter;

	class Material
	{
	public:
		~Material() {}

		static ptr<Material> create();

		void addTechnique(ptr<MaterialTechnique> technique);
		void removeTechnique(ptr<MaterialTechnique> technique);
		size_t getTechniquesCount() const;
		ptr<MaterialTechnique> getTechnique(unsigned index) const;
		
		void setCurrentTechnique(ptr<MaterialTechnique> technique);
		void setCurrentTechnique(unsigned index);
		ptr<MaterialTechnique> getCurrentTechnique() const;

		ptr<MaterialParameter> findParameter(const std::string &name);

	private:
		Material() {}

		ptr<MaterialTechnique> _currentTechnique;
		std::vector<ptr<MaterialTechnique>> _techniques;
		std::map<std::string, ptr<MaterialParameter>> _parameters;
	};
}
