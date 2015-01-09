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
		virtual ~ModelRenderer() override {}

		static ptr<ModelRenderer> create(size_t node);

		virtual void render() override;

		void setModel(const ptr<Model> model);
		ptr<Model> getModel() const;

		void setMaterial(unsigned index, ptr<Material> material);
		ptr<Material> getMaterial(unsigned index);
		size_t getMaterialCount() const;

	private:
		ModelRenderer(size_t node);

		ptr<Model> _model;
		std::map<unsigned, ptr<Material>> _materials;
	};
}