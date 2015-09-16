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
		auto modelUrl = "testCreateResourceAndTryToCleanIt";
		auto model = resourceManager->getOrCreateModel(modelUrl);
		resourceManager->cleanUnusedResources();
		assert(resourceManager->findModel(modelUrl) == model);
	}

	void test_CreateAndForgetResource_CleanIt()
	{
		auto modelUrl = "testCreateAndForgetResourceThenCleanIt";
		resourceManager->getOrCreateModel(modelUrl);
		resourceManager->cleanUnusedResources();
		assert(resourceManager->findModel(modelUrl) == nullptr);
	}
};
