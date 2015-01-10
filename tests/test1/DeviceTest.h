#pragma once

#include "TestBase.h"

using namespace solo;

class DeviceTest : public TestBase
{
public:
	DeviceTest(Engine* engine)
		: TestBase(engine)
	{
	}

	virtual void run() override
	{
		testCanvasSize();
	}

	void testCanvasSize()
	{
		auto size = _device->getCanvasSize();
		assert(size.x == 1366);
		assert(size.y == 768);
	}
};