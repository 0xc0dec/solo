#pragma once

#include "TestBase.h"


class Device_Test final: public TestBase
{
public:
    Device_Test(Device *device): TestBase(device)
    {
    }

    void run() override final
    {
        test_SetWindowTitle_GetWindowTitle();
    }

private:
    void test_SetWindowTitle_GetWindowTitle()
    {
        device->setWindowTitle("Test window");
        assert(device->getWindowTitle() == "Test window");
    }
};