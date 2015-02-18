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
		virtual void bind(RenderContext& context) override;
		virtual void unbind() override;

		Effect* getEffect();

	private:
		friend class MaterialPassFactory;

		MaterialPass(Material* material, shared<Effect> effect);
		MaterialPass(const MaterialPass& other) = delete;
		MaterialPass(MaterialPass&& other) = delete;
		MaterialPass& operator=(const MaterialPass& other) = delete;
		MaterialPass& operator=(MaterialPass&& other) = delete;

		Material *material;
		shared<Effect> effect;
	};

	class MaterialPassFactory
	{
		friend class Material;
		static shared<MaterialPass> create(Material* material, shared<Effect> effect);
	};
}