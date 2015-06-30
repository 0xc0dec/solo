#pragma once

#include "SoloBase.h"
#include "SoloRenderState.h"

namespace solo
{
	class Effect;
	class MaterialParameter;

	class Material: public RenderState
	{
	public:
		virtual void bind(RenderContext& context) override;
		virtual void unbind(RenderContext& context) override;

		MaterialParameter* getParameter(const std::string &name);
		Effect* getEffect() const;

	protected:
		Material(shared<Effect> effect);

	private:
		Material(const Material& other) = delete;
		Material(Material&& other) = delete;
		Material& operator=(const Material& other) = delete;
		Material& operator=(Material&& other) = delete;

		std::map<std::string, shared<MaterialParameter>> parameters;

		shared<Effect> effect;
	};

	class MaterialFactory
	{
		friend class ResourceManager;
		static shared<Material> create(shared<Effect> effect);
	};
}
