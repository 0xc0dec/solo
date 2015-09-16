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
	}

	void test_SetWindowTitle_GetWindowTitle()
	{
		engine->getDevice()->setWindowTitle("Test window");
		assert(engine->getDevice()->getWindowTitle() == "Test window");
	}
};