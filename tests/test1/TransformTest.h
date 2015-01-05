#pragma once

#include "TestBase.h"

class TransformTest : public TestBase
{
public:
	virtual void run(IEngine *engine) override
	{
		auto scene = engine->getScene();
		auto node = scene->createNode();
		auto transform = scene->getComponent<ITransform>(node);
		assert(transform);
	}
};
