#pragma once

#include "TestBase.h"


class TestComponent : public IComponent
{
public:
	DECLARE_COMPONENT(TestComponent)

	virtual void update() override
	{
	}
};


class TestComponent2 : public IComponent
{
public:
	DECLARE_COMPONENT(TestComponent2)
};


class ComponentsTest : public TestBase
{
public:
	virtual void run(IEngine* engine) override
	{
		testComponentsAddition(engine);
	}

	void testComponentsAddition(IEngine *engine)
	{
		auto scene = engine->getScene();
		auto node = scene->createEmptyNode();
		auto cmp = scene->addComponent<TestComponent>(node);
		auto cmp2 = scene->addComponent<TestComponent2>(node);
		assert(cmp);
		assert(cmp2);
		assert(cmp->getTypeId() == TestComponent::getComponentTypeId());
		assert(cmp2->getTypeId() == TestComponent2::getComponentTypeId());
	}
};
