#pragma once

#include "TestBase.h"

using namespace solo;

class DeviceTest : public TestBase
{
public:
	DeviceTest(Engine* engine):
		TestBase(engine)
	{
	}

	virtual void run() override
	{
		testWindowTitle();
		testCanvasSize();
	}

	void testCanvasSize()
	{
		auto size = device->getCanvasSize();
		assert(size.x == 640);
		assert(size.y == 480);
	}

	void testWindowTitle()
	{
		engine->getDevice()->setWindowTitle("Test window");
		assert(engine->getDevice()->getWindowTitle() == "Test window");
	}
};