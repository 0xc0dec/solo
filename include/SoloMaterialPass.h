#pragma once

#include "SoloBase.h"
#include "SoloRenderState.h"

namespace solo
{
	class Effect;
	class Material;

	class MaterialPass: public RenderState
	{
	public:
		MaterialPass(Material *material, ptr<Effect> effect);
		~MaterialPass() {}

		void bind();
		void unbind();

	private:
		Material *_material;
		ptr<Effect> _effect;
	};
}