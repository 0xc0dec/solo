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
		test_SetWindowTitle_GetWindowTitle();
		test_GetCanvasSize();
	}

	void test_GetCanvasSize()
	{
		auto size = device->getCanvasSize();
		assert(size.x == 640);
		assert(size.y == 480);
	}

	void test_SetWindowTitle_GetWindowTitle()
	{
		engine->getDevice()->setWindowTitle("Test window");
		assert(engine->getDevice()->getWindowTitle() == "Test window");
	}
};