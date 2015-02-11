#include "SoloModelRenderer.h"
#include "SoloModel.h"
#include "SoloMaterial.h"
#include "SoloMaterialPass.h"
#include "SoloMesh.h"

using namespace solo;


ModelRenderer::ModelRenderer(Node* node):
	ComponentBase(node)
{
}


ptr<ModelRenderer> ModelRenderer::create(Node* node)
{
	return NEW2(ModelRenderer, node);
}


void ModelRenderer::render(const RenderContext& context)
{
	auto meshCount = model->getMeshCount();
	for (auto i = 0; i < meshCount; ++i)
	{
		auto material = getMaterial(i);
		if (material)
		{
			auto mesh = model->getMesh(i);
			auto passCount = material->getPassCount();
			for (auto k = 0; k < passCount; ++k)
			{
				auto pass = material->getPass(k);
				pass->bind(context);
				mesh->draw();
				pass->unbind();
			}
		}
	}
}


void ModelRenderer::setModel(const ptr<Model> model)
{
	this->model = model;
}


ptr<Model> ModelRenderer::getModel() const
{
	return model;
}


void ModelRenderer::setMaterial(unsigned index, ptr<Material> material)
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


ptr<Material> ModelRenderer::getMaterial(unsigned index)
{
	if (materials.find(index) == materials.end())
		return nullptr;
	return materials[index];
}