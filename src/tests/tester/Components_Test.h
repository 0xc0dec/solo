/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "TestBase.h"


class A final: public ComponentBase<A>
{
public:
    explicit A(const Node &node): ComponentBase<A>(node)
    {
    }
};


class B final: public ComponentBase<B>
{
public:
    explicit B(const Node &node): ComponentBase<B>(node)
    {
    }
};


class C final: public ComponentBase<C>
{
public:
    Component *addedCmp = nullptr;
    Component *removedCmp = nullptr;

    explicit C(const Node &node) : ComponentBase<C>(node)
    {
    }

    void onComponentAdded(Component *cmp) override final
    {
        addedCmp = cmp;
    }

    void onComponentRemoved(Component *cmp) override final
    {
        removedCmp = cmp;
    }
};


class Base: public ComponentBase<Base>
{
public:
    explicit Base(const Node &node): ComponentBase<Base>(node)
    {
    }
};


class Derived final: public Base
{
public:
    explicit Derived(const Node &node): Base(node)
    {
    }
};


class Components_Test final: public TestBase
{
public:
    explicit Components_Test(Device *device): TestBase(device)
    {
    }

    void run() override final
    {
        test_AddComponents();
        test_AddDerivedComponent_EnsureFoundAsParent();
        test_AddComponent_Remove();
        test_AddAndRemove_FindByBaseId();
        test_RemoveInexistentComponent();
        test_AddOrRemoveComponents_EnsureAddedRemovedEventsCalled();
    }

private:
    void test_AddComponents() const
    {
        auto node = scene->createNode();
        auto a = node->addComponent<A>();
        auto b = node->addComponent<B>();
        assert(a);
        assert(b);
        assert(a->getTypeId() == A::getId());
        assert(b->getTypeId() == B::getId());
    }

    void test_AddOrRemoveComponents_EnsureAddedRemovedEventsCalled() const
    {
        auto node = scene->createNode();
        auto c = node->addComponent<C>();

        auto a = node->addComponent<A>();
        assert(c->addedCmp == a);

        auto b = node->addComponent<B>();
        assert(c->addedCmp == b);

        node->removeComponent<A>();
        assert(c->removedCmp == a);

        node->removeComponent<B>();
        assert(c->removedCmp == b);
    }

    void test_AddComponent_Remove() const
    {
        auto node = scene->createNode();
        node->addComponent<A>();
        node->removeComponent<A>();
        assert(node->findComponent<A>() == nullptr);
    }

    void test_AddAndRemove_FindByBaseId() const
    {
        auto node = scene->createNode();
        node->addComponent<Derived>();
        node->removeComponent<Base>();
        assert(node->findComponent<Derived>() == nullptr);
        assert(node->findComponent<Base>() == nullptr);
    }

    void test_RemoveInexistentComponent() const
    {
        auto node = scene->createNode();
        node->removeComponent<Derived>();
    }

    void test_AddDerivedComponent_EnsureFoundAsParent() const
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
};
