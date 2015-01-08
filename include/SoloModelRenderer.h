#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"

namespace solo
{
	class Model;
	class Material;

	class ModelRenderer: public ComponentBase<ModelRenderer>
	{
	public:
		ModelRenderer() {}
		virtual ~ModelRenderer() override {}

		virtual void render() override;

		void setModel(const ptr<Model> model);
		ptr<Model> getModel() const;

		void setMaterial(unsigned index, ptr<Material> material);
		ptr<Material> getMaterial(unsigned index);
		size_t getMaterialCount() const;

	private:
		ptr<Model> _model;
		std::map<unsigned, ptr<Material>> _materials;
	};
}