#ifndef __COMPONENTS_TEST_H__
#define __COMPONENTS_TEST_H__

#include "TestBase.h"

using namespace solo;


class TestComponent : public IComponent
{
public:
	DECLARE_CMP_ID(TestComponent)
};


class TestComponent2 : public IComponent
{
public:
	DECLARE_CMP_ID(TestComponent2)
};


class TestSystem : public ISystem
{
public:
	void update(size_t node, ptr<IComponent> component) override
	{
		assert(component->getTypeId() == TestComponent::getComponentTypeId());
	}
};


class ComponentsTest : public TestBase
{
public:
	virtual void run(solo::IEngine* engine) override
	{
		auto scene = engine->getScene();
		scene->addSystem<TestSystem>(TestComponent::getComponentTypeId());

		auto node = scene->createNode();
		auto cmp = scene->addComponent<TestComponent>(node);
		auto cmp2 = scene->addComponent<TestComponent2>(node);
		assert(cmp);
		assert(cmp2);
		assert(cmp->getTypeId() == TestComponent::getComponentTypeId());
		assert(cmp2->getTypeId() == TestComponent2::getComponentTypeId());
	}
};


#endif