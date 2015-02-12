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
		Material(const Material& other);
		Material(Material&& other);
		Material& operator=(const Material& other);
		Material& operator=(Material&& other);

		std::vector<ptr<MaterialPass>> passes;

		void bind(Effect* effect, const RenderContext& context);
	};

	class MaterialFactory
	{
		friend class ResourceManager;
		static ptr<Material> create();
	};
}
