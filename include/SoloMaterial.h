#pragma once

#include "SoloBase.h"
#include "SoloRenderState.h"

namespace solo
{
	class Effect;
	class MaterialPass;
	class RenderContext;

	class Material: public RenderState
	{
		friend MaterialPass;

	public:
		~Material() {}

		static ptr<Material> create();

		ptr<MaterialPass> addPass(ptr<Effect> effect);
		ptr<MaterialPass> getPass(unsigned index) const;
		void removePass(ptr<MaterialPass> pass);
		size_t getPassCount() const;

	private:
		Material() {}

		std::vector<ptr<MaterialPass>> _passes;

		void bind(ptr<Effect> effect, const RenderContext& context);
	};
}
