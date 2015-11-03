#pragma once

#include "TestBase.h"


class Resources_Test : public TestBase
{
public:
	Resources_Test(Device* device):
		TestBase(device)
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
		test_LoadCubeTextureWithWrongNumberOfFaces_EnsureFail();
		test_LoadTexture2DWithOverridenUri_CheckUri();
		test_LoadCubeTextureWithOverridenUri_CheckUri();
		test_LoadModelWithOverridenUri_CheckUri();
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

	void test_LoadCubeTextureWithWrongNumberOfFaces_EnsureFail()
	{
		assertThrows<EngineException>([=] { resourceManager->getOrLoadCubeTexture({ "1", "2" }); },
			"Wrong number of face images for cube texture (2 provided, 6 expected)");
	}

	void test_LoadTexture2DWithOverridenUri_CheckUri()
	{
		auto overridenUri = "customUriForTexture2D";
		auto tex = resourceManager->getOrLoadTexture2D("/image.png", overridenUri);
		assert(resourceManager->getOrLoadTexture2D("doesn't matter", overridenUri) == tex);
		assert(resourceManager->findTexture2D(overridenUri) == tex);
	}

	void test_LoadCubeTextureWithOverridenUri_CheckUri()
	{
		auto overridenUri = "customUriForCubeTexture";
		auto tex = resourceManager->getOrLoadCubeTexture({ "1", "2", "3", "4", "5", "6" }, overridenUri);
		assert(resourceManager->getOrLoadCubeTexture({ "12", "23", "34", "45", "56", "67" }, overridenUri) == tex);
		assert(resourceManager->findCubeTexture(overridenUri) == tex);
	}

	void test_LoadModelWithOverridenUri_CheckUri()
	{
		auto overridenUri = "customUriForModel";
		auto model = resourceManager->getOrLoadModel("/nonsense.obj", overridenUri);
		assert(resourceManager->getOrLoadModel("/more_nonsense.obj", overridenUri) == model);
		assert(resourceManager->findModel(overridenUri) == model);
	}
};
