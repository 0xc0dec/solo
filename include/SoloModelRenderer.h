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
		virtual ~ModelRenderer() {}

		virtual void render(RenderContext& context) override;

		void setModel(shared<Model> model);
		Model* getModel() const;

		void setMaterial(unsigned index, shared<Material> material);
		Material* getMaterial(unsigned index);
		size_t getMaterialCount() const;

	private:
		friend class ModelRendererFactory;

		ModelRenderer(Node* node);
		ModelRenderer(const ModelRenderer& other) = delete;
		ModelRenderer(ModelRenderer&& other) = delete;
		ModelRenderer& operator=(const ModelRenderer& other) = delete;
		ModelRenderer& operator=(ModelRenderer&& other) = delete;

		shared<Model> model;
		std::map<unsigned, shared<Material>> materials;
	};

	class ModelRendererFactory
	{
		friend class Node;
		static shared<ModelRenderer> create(Node *node);
	};
}