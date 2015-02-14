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

		MaterialPass* addPass(Effect* effect);
		MaterialPass* getPass(unsigned index) const;
		void removePass(MaterialPass* pass);
		size_t getPassCount() const;

	private:
		friend class MaterialFactory;

		Material() {}
		Material(const Material& other) = delete;
		Material(Material&& other) = delete;
		Material& operator=(const Material& other) = delete;
		Material& operator=(Material&& other) = delete;

		std::vector<shared<MaterialPass>> passes;

		void bind(Effect* effect, const RenderContext& context);
	};

	class MaterialFactory
	{
		friend class ResourceManager;
		static shared<Material> create();
	};
}
