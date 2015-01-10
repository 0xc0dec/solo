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
		_node = _scene->createEmptyNode();
		testComponentsAddition();
		testDerivedComponents();
	}

	void testComponentsAddition()
	{
		auto a = _scene->addComponent<A>(_node);
		auto b = _scene->addComponent<B>(_node);
		assert(a);
		assert(b);
		assert(a->getTypeId() == A::getId());
		assert(b->getTypeId() == B::getId());
	}

	void testDerivedComponents()
	{
		_scene->addComponent<Derived>(_node);
		auto base = _scene->getComponent<Base>(_node);
		auto derived = _scene->getComponent<Derived>(_node);
		assert(base);
		assert(derived);
		assert(base == derived);
		assert(base->getTypeId() == Derived::getId());
		assert(base->getTypeId() == Base::getId());
		assert(derived->getTypeId() == base->getTypeId());
	}

private:
	size_t _node;
};
