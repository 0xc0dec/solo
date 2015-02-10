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
		auto transform = scene->getComponent<Transform>(node);
		assert(transform);
	}

	void testParentAndChildren()
	{
		auto t1 = scene->getComponent<Transform>(scene->createNode());
		auto t2 = scene->getComponent<Transform>(scene->createNode());
		auto t3 = scene->getComponent<Transform>(scene->createNode());

		assert(t1->getChildrenCount() == 0);

		t2->setParent(t1.get());
		assert(t2->getParent() == t1.get());
		assert(t1->getChildrenCount() == 1);
		assert(t1->getChild(0) == t2.get());

		t3->setParent(t1.get());
		assert(t1->getChildrenCount() == 2);
		assert(t1->getChild(0) == t2.get());
		assert(t1->getChild(1) == t3.get());

		std::list<Transform*> untouched = { t2.get(), t3.get() };
		t1->iterateChildren([&untouched](Transform *t) { untouched.remove(t); });
		assert(untouched.empty());
	}
};
