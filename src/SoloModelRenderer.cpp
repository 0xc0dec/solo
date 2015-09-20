#include "SoloModelRenderer.h"
#include "SoloModel.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"
#include "SoloRenderContext.h"

using namespace solo;


shared<ModelRenderer> ModelRendererFactory::create(Node node)
{
	return NEW2(ModelRenderer, node);
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
	materials.clear();
}


Model* ModelRenderer::getModel() const
{
	return model.get();
}


void ModelRenderer::setMaterial(unsigned index, shared<Material> material)
{
	if (!model)
		THROW_FMT(EngineException, "Renderer has no model, hence setting material has no effect");
	if (index >= model->getMeshCount())
		THROW_FMT(EngineException, "Trying to set material with index ", index, ", but model has only ", model->getMeshCount(), " meshes");
	if (!material)
		materials.erase(index);
	else
		materials[index] = material;
}


void ModelRenderer::setMaterial(shared<Material> material)
{
	if (!material)
		materials.clear();
	else
	{
		auto meshCount = model->getMeshCount();
		for (auto i = 0; i < meshCount; ++i)
			setMaterial(i, material);
	}
}


size_t ModelRenderer::getMaterialCount() const
{
	return materials.size();
}


Material* ModelRenderer::getMaterial(unsigned index) const
{
	if (materials.find(index) == materials.end())
		return nullptr;
	return materials.at(index).get();
}
