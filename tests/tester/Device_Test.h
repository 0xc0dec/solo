#pragma once

#include "TestBase.h"


class Device_Test : public TestBase
{
public:
    Device_Test(Device *device):
        TestBase(device)
    {
    }

    virtual void run() override
    {
        test_SetWindowTitle_GetWindowTitle();
    }

    void test_SetWindowTitle_GetWindowTitle()
    {
        device->setWindowTitle("Test window");
        assert(device->getWindowTitle() == "Test window");
    }
};