#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloNode.h"

namespace solo
{
	class Model;
	class Material;

	class ModelRenderer: public ComponentBase<ModelRenderer>
	{
	public:
		virtual void render(RenderContext& context) override;

		void setModel(shared<Model> model);
		Model* getModel() const;

		void setMaterial(unsigned index, shared<Material> material);
		void setMaterial(shared<Material> material);
		Material* getMaterial(unsigned index) const;
		size_t getMaterialCount() const;

	private:
		friend class ModelRendererFactory;

		explicit ModelRenderer(Node node): ComponentBase(node) {}

		shared<Model> model;
		std::unordered_map<unsigned, shared<Material>> materials;
	};

	class ModelRendererFactory
	{
		friend class Node;
		static shared<ModelRenderer> create(Node node);
	};
}