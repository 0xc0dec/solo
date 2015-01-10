#pragma once

#include "TestBase.h"

class TransformTest : public TestBase
{
public:
	TransformTest(Engine* engine)
		: TestBase(engine)
	{
	}

	virtual void run() override
	{
		auto node = _scene->createNode();
		auto transform = _scene->getComponent<Transform>(node);
		assert(transform);
	}
};
