#pragma once

#include "TestBase.h"


class ModelRenderer_Test : public TestBase
{
public:
	ModelRenderer_Test(Device* device) :
		TestBase(device)
	{
	}

	virtual void run() override
	{
		renderer = scene->createNode()->addComponent<ModelRenderer>();
		material = resourceManager->getOrCreateMaterial(resourceManager->getOrCreateEffect("1", "2"));
		auto mesh = resourceManager->getOrCreateMesh();
		model = resourceManager->getOrCreateModel();
		model->addMesh(mesh);
		model->addMesh(mesh);
		model->addMesh(mesh);

		test_EnsureNoMaterialsAtFirst();
		test_SetModel_UnsetModel();
		test_SetMaterialForVariousIndexes();
		test_SetMaterial_EnsureSetForAllIndices();
	}

	void test_SetMaterial_EnsureSetForAllIndices()
	{
		renderer->setModel(model);
		renderer->setMaterial(material);
		assert(renderer->getMaterial(0) == material.get());
		assert(renderer->getMaterial(1) == material.get());
		assert(renderer->getMaterial(2) == material.get());
		assert(renderer->getMaterialCount() == 3);
	}

	void test_SetMaterialForVariousIndexes()
	{
		renderer->setModel(model);
		renderer->setMaterialForMesh(0, material);
		renderer->setMaterialForMesh(1, material);
		renderer->setMaterialForMesh(2, material);
		assertThrows<EngineException>([=]() { renderer->setMaterialForMesh(3, material); });
	}

	void test_EnsureNoMaterialsAtFirst()
	{
		assert(renderer->getMaterialCount() == 0);
	}

	void test_SetModel_UnsetModel()
	{
		renderer->setModel(model);
		assert(renderer->getModel() == model.get());
		assert(renderer->getMaterialCount() == 0); // materials get reset

		renderer->setModel(nullptr);
		assert(renderer->getModel() == nullptr);
	}

private:
	shared<Model> model{ nullptr };
	shared<Material> material{ nullptr };
	ModelRenderer* renderer{ nullptr };
};