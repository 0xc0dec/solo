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
		friend Material;

	public:
		~MaterialPass() {}

		void bind(const RenderContext& context);
		void unbind();

	private:
		friend class MaterialPassFactory;

		MaterialPass(Material *material, ptr<Effect> effect);

		Material *material;
		ptr<Effect> effect;
	};

	class MaterialPassFactory
	{
		friend class Material;
		static ptr<MaterialPass> create(Material *material, ptr<Effect> effect);
	};
}