#include "SoloModelRenderer.h"
#include "SoloModel.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"
#include "SoloRenderContext.h"

using namespace solo;


shared<ModelRenderer> ModelRendererFactory::create(Scene* scene, size_t nodeId)
{
	return NEW2(ModelRenderer, scene, nodeId);
}


ModelRenderer::ModelRenderer(Scene* scene, size_t nodeId) :
	ComponentBase(scene, nodeId)
{
}


void ModelRenderer::render(RenderContext& context)
{
	auto meshCount = model->getMeshCount();
	for (auto i = 0; i < meshCount; ++i)
	{
		auto material = getMaterial(i);
		if (material)
		{
			auto mesh = model->getMesh(i);
			material->bind(context);
			mesh->draw();
			material->unbind(context);
		}
	}
}


void ModelRenderer::setModel(shared<Model> model)
{
	this->model = model;
}


Model* ModelRenderer::getModel() const
{
	return model.get();
}


void ModelRenderer::setMaterial(unsigned index, shared<Material> material)
{
	if (!material)
		materials.erase(index);
	else
		materials[index] = material;
}


size_t ModelRenderer::getMaterialCount() const
{
	return materials.size();
}


Material* ModelRenderer::getMaterial(unsigned index)
{
	if (materials.find(index) == materials.end())
		return nullptr;
	return materials[index].get();
}
