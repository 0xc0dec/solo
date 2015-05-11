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
		testCreateResourceAndTryToCleanIt();
		testCreateAndForgetResourceThenCleanIt();
	}

	void testCreateResourceAndTryToCleanIt()
	{
		auto mgr = engine->getResourceManager();
		auto modelUrl = "testCreateResourceAndTryToCleanIt";
		auto model = mgr->getOrCreateModel(modelUrl);
		mgr->cleanUnusedResources();
		assert(mgr->findModel(modelUrl) == model);
	}

	void testCreateAndForgetResourceThenCleanIt()
	{
		auto mgr = engine->getResourceManager();
		auto modelUrl = "testCreateAndForgetResourceThenCleanIt";
		mgr->getOrCreateModel(modelUrl);
		mgr->cleanUnusedResources();
		assert(mgr->findModel(modelUrl) == nullptr);
	}
};
