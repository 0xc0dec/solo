#pragma once

#include "TestBase.h"


class Resources_Test : public TestBase
{
public:
	Resources_Test(Engine* engine):
		TestBase(engine)
	{
	}

	virtual void run() override
	{
		test_FindInexistentResources_EnsureNotFound();
		test_CreateEffect_FindIt();
		test_CreateTexture2D_FindIt();
		test_CreateMaterial_FindIt();
		test_CreateMesh_FindIt();
		test_CreateModel_FindIt();
		test_CreateRenderTarget_FindIt();
		test_CreateResource_TryToCleanIt_EnsureRemains();
		test_CreateAndForgetResource_CleanIt();
	}

	void test_FindInexistentResources_EnsureNotFound()
	{
		assert(!resourceManager->findEffect("non existent"));
		assert(!resourceManager->findMaterial("non existent"));
		assert(!resourceManager->findMesh("non existent"));
		assert(!resourceManager->findModel("non existent"));
		assert(!resourceManager->findRenderTarget("non existent"));
		assert(!resourceManager->findTexture2D("non existent"));
	}

	void test_CreateEffect_FindIt()
	{
		auto uri = "effect";
		auto vs = "abc";
		auto fs = "def";
		auto eff = resourceManager->getOrCreateEffect(vs, fs, uri);
		assert(resourceManager->findEffect(uri) == eff);
		assert(resourceManager->getOrCreateEffect(vs, fs, uri) == eff);
	}

	void test_CreateTexture2D_FindIt()
	{
		auto uri = "tex2d";
		auto tex = resourceManager->getOrCreateTexture2D(uri);
		assert(resourceManager->findTexture2D(uri) == tex);
		assert(resourceManager->getOrCreateTexture2D(uri) == tex);
	}

	void test_CreateMaterial_FindIt()
	{
		auto uri = "mat";
		auto eff = resourceManager->getOrCreateEffect("1", "2");
		auto mat = resourceManager->getOrCreateMaterial(eff, uri);
		assert(resourceManager->findMaterial(uri) == mat);
		assert(resourceManager->getOrCreateMaterial(eff, uri) == mat);
	}

	void test_CreateMesh_FindIt()
	{
		auto uri = "mesh";
		auto mesh = resourceManager->getOrCreateMesh(uri);
		assert(resourceManager->findMesh(uri) == mesh);
		assert(resourceManager->getOrCreateMesh(uri) == mesh);
	}

	void test_CreateModel_FindIt()
	{
		auto uri = "model";
		auto model = resourceManager->getOrCreateModel(uri);
		assert(resourceManager->findModel(uri) == model);
		assert(resourceManager->getOrCreateModel(uri) == model);
	}

	void test_CreateRenderTarget_FindIt()
	{
		auto uri = "rt";
		auto rt = resourceManager->getOrCreateRenderTarget(uri);
		assert(resourceManager->findRenderTarget(uri) == rt);
		assert(resourceManager->getOrCreateRenderTarget(uri) == rt);
	}

	void test_CreateResource_TryToCleanIt_EnsureRemains()
	{
		auto modelUri = "testCreateResourceAndTryToCleanIt";
		auto model = resourceManager->getOrCreateModel(modelUri);
		resourceManager->cleanUnusedResources();
		assert(resourceManager->findModel(modelUri) == model);
	}

	void test_CreateAndForgetResource_CleanIt()
	{
		auto modelUri = "testCreateAndForgetResourceThenCleanIt";
		resourceManager->getOrCreateModel(modelUri);
		resourceManager->cleanUnusedResources();
		assert(resourceManager->findModel(modelUri) == nullptr);
	}
};
