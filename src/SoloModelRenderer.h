#pragma once

#include "SoloBaseInternal.h"
#include "SoloIModelRenderer.h"

namespace solo
{
	class Model;

	class ModelRenderer: public IModelRenderer
	{
	public:
		static ptr<ModelRenderer> create();

		virtual void render() override;

		virtual void setModel(const ptr<IModel> model) override;
		virtual ptr<IModel> getModel() override;

	private:
		ModelRenderer() {}
		virtual ~ModelRenderer() {}

		ptr<Model> _model;
	};
}