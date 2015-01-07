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

		static ptr<ModelRenderer> create();

		virtual void render() override;

		void setModel(const ptr<Model> model);
		ptr<Model> getModel() const;

		void addMaterial(ptr<Material> material);
		void removeMaterial(ptr<Material> material);
		ptr<Material> getMaterial(unsigned index);
		size_t getMaterialCount() const;

	private:
		ModelRenderer();

		ptr<Model> _model;
		// TODO make in map and add method setMaterial(unsigned index, Material *m)
		// instead of the addMaterial method
		std::vector<ptr<Material>> _materials;
	};
}