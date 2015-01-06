#include "SoloModelRenderer.h"
#include "SoloModel.h"

using namespace solo;


std::shared_ptr<solo::ModelRenderer> ModelRenderer::create()
{
	return NEW2(ModelRenderer);
}


void ModelRenderer::render()
{

}


void ModelRenderer::setModel(const ptr<Model> model)
{
	_model = model;
}


std::shared_ptr<solo::Model> ModelRenderer::getModel()
{
	return _model;
}