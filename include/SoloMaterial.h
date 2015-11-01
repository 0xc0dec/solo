#pragma once

#include "SoloBase.h"
#include "SoloRenderState.h"

namespace solo
{
	class Effect;
	class MaterialParameter;
	enum class EngineMode;

	class Material: public RenderState
	{
	public:
		virtual void bind(RenderContext& context) override;
		virtual void unbind(RenderContext& context) override;

		MaterialParameter* getParameter(const std::string &name);
		Effect* getEffect() const;

	protected:
		explicit Material(shared<Effect> effect);

	private:
		shared<Effect> effect;
		std::unordered_map<std::string, shared<MaterialParameter>> parameters;
	};

	inline Effect* Material::getEffect() const
	{
		return effect.get();
	}

	class MaterialFactory
	{
		friend class ResourceManager;
		static shared<Material> create(EngineMode mode, shared<Effect> effect);
	};
}
