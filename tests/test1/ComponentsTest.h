#pragma once

#include "TestBase.h"


class A : public ComponentBase<A>
{
};


class B: public ComponentBase<B>
{
};


class Base : public ComponentBase<Base>
{
};


class Derived : public Base
{
};


class ComponentsTest : public TestBase
{
public:
	virtual void run(Engine* engine) override
	{
		_scene = engine->getScene();
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
	ptr<Scene> _scene;
	size_t _node;
};
