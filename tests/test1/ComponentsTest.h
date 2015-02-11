#pragma once

#include "TestBase.h"


class A : public ComponentBase<A>
{
public:
	explicit A(Node* node):
		ComponentBase<A>(node)
	{
	}
};


class B: public ComponentBase<B>
{
public:
	explicit B(Node* node):
		ComponentBase<B>(node)
	{
	}
};


class Base : public ComponentBase<Base>
{
public:
	explicit Base(Node* node):
		ComponentBase<Base>(node)
	{
	}
};


class Derived : public Base
{
public:
	Derived(Node* node) :
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
		auto a = node->addComponent<A>();
		auto b = node->addComponent<B>();
		assert(a);
		assert(b);
		assert(a->getTypeId() == A::getId());
		assert(b->getTypeId() == B::getId());
	}

	void testDerivedComponents()
	{
		node->addComponent<Derived>();
		auto base = node->getComponent<Base>();
		auto derived = node->getComponent<Derived>();
		assert(base);
		assert(derived);
		assert(base == derived);
		assert(base->getTypeId() == Derived::getId());
		assert(base->getTypeId() == Base::getId());
		assert(derived->getTypeId() == base->getTypeId());
	}

private:
	Node* node;
};
