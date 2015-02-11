#pragma once

#include "TestBase.h"

class TransformTest : public TestBase
{
public:
	TransformTest(Engine* engine):
		TestBase(engine)
	{
	}

	virtual void run() override
	{
		testGetTransform();
		testParentAndChildren();
	}

	void testGetTransform()
	{
		auto node = scene->createNode();
		auto transform = node->getComponent<Transform>();
		assert(transform);
	}

	void testParentAndChildren()
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
