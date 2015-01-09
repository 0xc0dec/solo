#include "SoloModelRenderer.h"
#include "SoloModel.h"
#include "SoloMaterial.h"
#include "SoloMaterialPass.h"
#include "SoloMesh.h"

using namespace solo;


ModelRenderer::ModelRenderer(size_t node):
	ComponentBase(node)
{
}


ptr<ModelRenderer> ModelRenderer::create(size_t node)
{
	return NEW2(ModelRenderer, node);
}


void ModelRenderer::render(const RenderContext& context)
{
	auto meshCount = _model->getMeshCount();
	for (auto i = 0; i < meshCount; ++i)
	{
		auto material = getMaterial(i);
		if (material)
		{
			auto mesh = _model->getMesh(i);
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
	_model = model;
}


ptr<Model> ModelRenderer::getModel() const
{
	return _model;
}


void ModelRenderer::setMaterial(unsigned index, ptr<Material> material)
{
	if (!material)
		_materials.erase(index);
	else
		_materials[index] = material;
}


size_t ModelRenderer::getMaterialCount() const
{
	return _materials.size();
}


ptr<Material> ModelRenderer::getMaterial(unsigned index)
{
	if (_materials.find(index) == _materials.end())
		return nullptr;
	return _materials[index];
}