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

		virtual void render(const RenderContext& context) override;

		void setModel(Model* model);
		Model* getModel() const;

		void setMaterial(unsigned index, Material* material);
		Material* getMaterial(unsigned index);
		size_t getMaterialCount() const;

	private:
		friend class ModelRendererFactory;

		ModelRenderer(Node* node);

		Model* model;
		std::map<unsigned, Material*> materials;
	};

	class ModelRendererFactory
	{
		friend class Node;
		static ptr<ModelRenderer> create(Node *node);
	};
}