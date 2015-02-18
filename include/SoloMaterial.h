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
	public:
		MaterialPass* addPass(shared<Effect> effect);
		MaterialPass* getPass(unsigned index) const;
		void removePass(MaterialPass* pass);
		size_t getPassCount() const;

		virtual void bind(RenderContext& context) override;
		virtual void unbind(RenderContext& context) override;

	protected:
		Material() = default;
		virtual void applyFaceCull() = 0;

	private:
		friend class MaterialFactory;
		friend class ModelRenderer;
		friend class MaterialPass;

		Material(const Material& other) = delete;
		Material(Material&& other) = delete;
		Material& operator=(const Material& other) = delete;
		Material& operator=(Material&& other) = delete;

		std::vector<shared<MaterialPass>> passes;
	};

	class MaterialFactory
	{
		friend class ResourceManager;
		static shared<Material> create();
	};
}
