#pragma once

#include "SoloBase.h"
#include "SoloRenderState.h"
#include "SoloMaterialPass.h"

namespace solo
{
	class Effect;

	class Material: public RenderState
	{
	public:
		~Material() {}

		static ptr<Material> create();

		ptr<MaterialPass> addPass(ptr<Effect> effect);
		ptr<MaterialPass> getPass(unsigned index) const;
		void removePass(ptr<MaterialPass> pass);
		size_t getPassCount() const;

	private:
		friend void MaterialPass::bind();

		Material() {}

		std::vector<ptr<MaterialPass>> _passes;

		void bind(ptr<Effect> effect);
	};
}
