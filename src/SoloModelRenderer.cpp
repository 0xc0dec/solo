#include "SoloModelRenderer.h"
#include "SoloModel.h"


std::shared_ptr<solo::ModelRenderer> solo::ModelRenderer::create()
{
	return ALLOC_WITH_DELETER(ModelRenderer);
}


void solo::ModelRenderer::render()
{

}


void solo::ModelRenderer::setModel(const ptr<IModel> model)
{
	_model = CAST_PTR_STATIC<Model>(model);
}


std::shared_ptr<solo::IModel> solo::ModelRenderer::getModel()
{
	return _model;
}