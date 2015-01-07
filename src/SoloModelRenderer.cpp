#include "SoloModelRenderer.h"
#include "SoloModel.h"
#include "SoloMaterial.h"
#include "SoloMaterialPass.h"
#include "SoloMesh.h"

using namespace solo;


ModelRenderer::ModelRenderer()
{
}


ptr<ModelRenderer> ModelRenderer::create()
{
	return NEW2(ModelRenderer);
}


void ModelRenderer::render()
{
	auto materialCount = getMaterialCount();
	auto meshCount = _model->getMeshCount();
	ptr<Material> material = nullptr;
	for (auto i = 0; i < meshCount; ++i)
	{
		if (i < materialCount)
			material = getMaterial(i);
		if (!material)
			break;
		material->bind();
		auto mesh = _model->getMesh(i);
		auto passCount = material->getPassCount();
		for (auto k = 0; k < passCount; ++k)
		{
			auto pass = material->getPass(k);
			pass->bind();
			mesh->draw();
			pass->unbind();
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


void ModelRenderer::addMaterial(ptr<Material> material)
{
	_materials.push_back(material);
}


void ModelRenderer::removeMaterial(ptr<Material> material)
{
	auto where = find(_materials.begin(), _materials.end(), material);
	if (where != _materials.end())
		_materials.erase(where);
}


size_t ModelRenderer::getMaterialCount() const
{
	return _materials.size();
}


ptr<Material> ModelRenderer::getMaterial(unsigned index)
{
	return _materials[index];
}