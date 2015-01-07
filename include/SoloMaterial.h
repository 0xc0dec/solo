#pragma once

#include "SoloBase.h"

namespace solo
{
	class MaterialParameter;
	class MaterialPass;

	class Material
	{
	public:
		~Material() {}

		static ptr<Material> create();

		void addPass(ptr<MaterialPass> pass);
		void removePass(ptr<MaterialPass> pass);
		size_t getPassCount() const;
		ptr<MaterialPass> getPass(unsigned index) const;
		
		ptr<MaterialParameter> findParameter(const std::string &name);

		void bind();

	private:
		Material() {}

		std::vector<ptr<MaterialPass>> _passes;
		std::map<std::string, ptr<MaterialParameter>> _parameters;
	};
}
