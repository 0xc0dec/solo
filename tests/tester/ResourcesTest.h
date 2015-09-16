#pragma once

#include "TestBase.h"

class ResourcesTest : public TestBase
{
public:
	ResourcesTest(Engine* engine):
		TestBase(engine)
	{
	}

	virtual void run() override
	{
		test_CreateResource_TryToCleanIt_EnsureRemains();
		test_CreateAndForgetResource_CleanIt();
	}

	void test_CreateResource_TryToCleanIt_EnsureRemains()
	{
		auto mgr = engine->getResourceManager();
		auto modelUrl = "testCreateResourceAndTryToCleanIt";
		auto model = mgr->getOrCreateModel(modelUrl);
		mgr->cleanUnusedResources();
		assert(mgr->findModel(modelUrl) == model);
	}

	void test_CreateAndForgetResource_CleanIt()
	{
		auto mgr = engine->getResourceManager();
		auto modelUrl = "testCreateAndForgetResourceThenCleanIt";
		mgr->getOrCreateModel(modelUrl);
		mgr->cleanUnusedResources();
		assert(mgr->findModel(modelUrl) == nullptr);
	}
};
