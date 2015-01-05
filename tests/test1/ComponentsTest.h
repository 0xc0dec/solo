#pragma once

#include "TestBase.h"


class A : public Component<A>
{
};


class B: public Component<B>
{
};


class Base : public Component<Base>
{
};


class Derived : public Base
{
};


class ComponentsTest : public TestBase
{
public:
	virtual void run(IEngine* engine) override
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
		assert(a->getComponentTypeId() == A::getId());
		assert(b->getComponentTypeId() == B::getId());
	}

	void testDerivedComponents()
	{
		_scene->addComponent<Derived>(_node);
		auto base = _scene->getComponent<Base>(_node);
		auto derived = _scene->getComponent<Derived>(_node);
		assert(base);
		assert(derived);
		assert(base == derived);
		assert(base->getComponentTypeId() == Derived::getId());
		assert(base->getComponentTypeId() == Base::getId());
		assert(derived->getComponentTypeId() == base->getComponentTypeId());
	}

private:
	ptr<IScene> _scene;
	size_t _node;
};
