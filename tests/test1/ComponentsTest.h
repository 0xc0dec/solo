#pragma once

#include "TestBase.h"


class A : public ComponentBase<A>
{
public:
	explicit A(size_t node):
		ComponentBase<A>(node)
	{
	}
};


class B: public ComponentBase<B>
{
public:
	explicit B(size_t node):
		ComponentBase<B>(node)
	{
	}
};


class Base : public ComponentBase<Base>
{
public:
	explicit Base(size_t node):
		ComponentBase<Base>(node)
	{
	}
};


class Derived : public Base
{
public:
	Derived(size_t node):
		Base(node)
	{
	}
};


class ComponentsTest : public TestBase
{
public:
	ComponentsTest(Engine* engine)
		: TestBase(engine)
	{
	}

	virtual void run() override
	{
		node = scene->createEmptyNode();
		testComponentsAddition();
		testDerivedComponents();
	}

	void testComponentsAddition()
	{
		auto a = scene->addComponent<A>(node);
		auto b = scene->addComponent<B>(node);
		assert(a);
		assert(b);
		assert(a->getTypeId() == A::getId());
		assert(b->getTypeId() == B::getId());
	}

	void testDerivedComponents()
	{
		scene->addComponent<Derived>(node);
		auto base = scene->getComponent<Base>(node);
		auto derived = scene->getComponent<Derived>(node);
		assert(base);
		assert(derived);
		assert(base == derived);
		assert(base->getTypeId() == Derived::getId());
		assert(base->getTypeId() == Base::getId());
		assert(derived->getTypeId() == base->getTypeId());
	}

private:
	size_t node;
};
