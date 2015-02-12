#pragma once

#include "SoloBase.h"
#include "SoloRenderState.h"
#include "SoloMaterial.h"

namespace solo
{
	class Effect;
	class RenderContext;

	class MaterialPass: public RenderState
	{
	public:
		~MaterialPass() {}

		void bind(const RenderContext& context);
		void unbind();

	private:
		friend Material;

		MaterialPass(Material *material, Effect* effect);

		Material *material;
		Effect* effect;
	};
}