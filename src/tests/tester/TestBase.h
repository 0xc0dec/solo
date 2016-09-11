/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "../../../include/solo.h"
#include <iostream>
#include <cassert>

#define LOG(msg) std::cout << msg << std::endl;

using namespace solo;


class TestBase
{
public:
    TestBase(Device *device) :
        device(device),
        scene(device->getScene()),
        fileSystem(device->getFileSystem())
    {
    }

    virtual ~TestBase() {}

    virtual void run() = 0;

protected:
    template <typename T>
    void assertThrows(std::function<void(void)> code, const std::string &expectedMsg = "")
    {
        try
        {
            code();
            assert(false);
        }
        catch (T &e)
        {
            if (!expectedMsg.empty())
                assert(expectedMsg == e.what());
        }
        catch (...)
        {
            assert(false);
        }
    }

    Device *device;
    Scene *scene;
    FileSystem *fileSystem;
};
