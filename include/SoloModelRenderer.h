#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"

namespace solo
{
	class Model;

	class ModelRenderer: public ComponentBase<ModelRenderer>
	{
	public:
		virtual ~ModelRenderer() override {}

		static ptr<ModelRenderer> create();

		virtual void render() override;

		void setModel(const ptr<Model> model);
		ptr<Model> getModel();

	private:
		ModelRenderer() {}

		ptr<Model> _model;
	};
}