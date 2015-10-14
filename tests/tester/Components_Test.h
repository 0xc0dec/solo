#pragma once

#include "TestBase.h"


class A : public ComponentBase<A>
{
public:
	explicit A(Node node): ComponentBase<A>(node)
	{
	}
};


class B: public ComponentBase<B>
{
public:
	explicit B(Node node): ComponentBase<B>(node)
	{
	}
};


class Base : public ComponentBase<Base>
{
public:
	explicit Base(Node node): ComponentBase<Base>(node)
	{
	}
};


class Derived : public Base
{
public:
	Derived(Node node): Base(node)
	{
	}
};


class ComponentsAndNodes_Test : public TestBase
{
public:
	ComponentsAndNodes_Test(Engine* engine): TestBase(engine)
	{
	}

	virtual void run() override
	{
		scene->clear();
		test_AddComponents_EnsureAdded();
		test_AddDerivedComponent_EnsureFoundAsParentAsWell();
		test_AddComponent_RemoveIt();
		test_AddComponent_RemoveAsParent();
		test_RemoveInexistentComponent_EnsureNothingDone();
		test_RemoveAllComponents_EnsureRemoved();
		test_AddDuplicateComponent_EnsureError();
	}

private:
	void test_AddComponents_EnsureAdded()
	{
		auto node = scene->createNode();
		auto a = node->addComponent<A>();
		auto b = node->addComponent<B>();
		assert(a);
		assert(b);
		assert(a->getTypeId() == A::getId());
		assert(b->getTypeId() == B::getId());
	}

	void test_RemoveAllComponents_EnsureRemoved()
	{
		auto node = scene->createNode();
		node->addComponent<Derived>();
		node->addComponent<A>();
		node->addComponent<B>();
		node->removeAllComponents();
		assert(node->findComponent<Derived>() == nullptr);
		assert(node->findComponent<A>() == nullptr);
		assert(node->findComponent<B>() == nullptr);
	}

	void test_AddComponent_RemoveIt()
	{
		auto node = scene->createNode();
		node->addComponent<A>();
		node->removeComponent<A>();
		assert(node->findComponent<A>() == nullptr);
	}

	void test_AddComponent_RemoveAsParent()
	{
		auto node = scene->createNode();
		node->addComponent<Derived>();
		node->removeComponent<Base>();
		assert(node->findComponent<Derived>() == nullptr);
		assert(node->findComponent<Base>() == nullptr);
	}

	void test_RemoveInexistentComponent_EnsureNothingDone()
	{
		auto node = scene->createNode();
		node->removeComponent<Base>();
		node->removeComponent<Derived>();
	}

	void test_AddDerivedComponent_EnsureFoundAsParentAsWell()
	{
		auto node = scene->createNode();
		node->addComponent<Derived>();
		auto base = node->findComponent<Base>();
		auto derived = node->findComponent<Derived>();
		assert(base);
		assert(derived);
		assert(base == derived);
		assert(base->getTypeId() == Derived::getId());
		assert(base->getTypeId() == Base::getId());
		assert(derived->getTypeId() == base->getTypeId());
	}

	void test_AddDuplicateComponent_EnsureError()
	{
		auto node = scene->createNode();
		node->addComponent<Base>();
		try
		{
			node->addComponent<Derived>();
		}
		catch (const EngineException& e)
		{
			assert(std::string(e.what()).find("already exists") != std::string::npos);
			return;
		}
		assert(false);
	}
};
