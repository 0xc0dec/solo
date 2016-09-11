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

#include "TestBase.h"


class Transform_Test final : public TestBase
{
public:
    Transform_Test(Device *device) : TestBase(device)
    {
    }

    void run() override final
    {
        test_GetTransformComponent();
        test_ChildrenManipulation();
    }

private:
    void test_GetTransformComponent()
    {
        auto node = scene->createNode();
        auto transform = node->findComponent<Transform>();
        assert(transform);
    }

    void test_ChildrenManipulation()
    {
        auto t1 = scene->createNode()->findComponent<Transform>();
        auto t2 = scene->createNode()->findComponent<Transform>();
        auto t3 = scene->createNode()->findComponent<Transform>();

        assert(t1->getChildrenCount() == 0);

        t2->setParent(t1);
        assert(t2->getParent() == t1);
        assert(t1->getChildrenCount() == 1);
        assert(t1->getChild(0) == t2);

        t3->setParent(t1);
        assert(t1->getChildrenCount() == 2);
        assert(t1->getChild(0) == t2);
        assert(t1->getChild(1) == t3);
    }
};
