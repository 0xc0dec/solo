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
        auto transform = node->getComponent<Transform>();
        assert(transform);
    }

    void test_ChildrenManipulation()
    {
        auto t1 = scene->createNode()->getComponent<Transform>();
        auto t2 = scene->createNode()->getComponent<Transform>();
        auto t3 = scene->createNode()->getComponent<Transform>();

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
