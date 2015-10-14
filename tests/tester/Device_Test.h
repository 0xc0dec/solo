#pragma once

#include "TestBase.h"


class Device_Test : public TestBase
{
public:
	Device_Test(Engine* engine):
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