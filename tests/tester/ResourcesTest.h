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
