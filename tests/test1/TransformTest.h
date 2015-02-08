#pragma once

#include "TestBase.h"

class TransformTest : public TestBase
{
public:
	TransformTest(Engine* engine):
		TestBase(engine)
	{
	}

	virtual void run() override
	{
		auto node = scene->createNode();
		auto transform = scene->getComponent<Transform>(node);
		assert(transform);
	}
};
