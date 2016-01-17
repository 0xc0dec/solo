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


class CallbackCaller: public ComponentBase<CallbackCaller>
{
public:
    explicit CallbackCaller(Node node, std::function<void()> initAction, std::function<void()> updateAction, std::function<void()> terminateAction):
        ComponentBase<CallbackCaller>(node),
        initAction(initAction),
        updateAction(updateAction),
        terminateAction(terminateAction)
    {
    }

    virtual void init() override
    {
        if (initAction)
            initAction();
    }

    virtual void update() override
    {
        if (updateAction)
            updateAction();
    }

    virtual void terminate() override
    {
        if (terminateAction)
            terminateAction();
    }

private:
    std::function<void()> initAction;
    std::function<void()> updateAction;
    std::function<void()> terminateAction;
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
    ComponentsAndNodes_Test(Device *device): TestBase(device)
    {
    }

    virtual void run() override
    {
        test_AddComponents();
        test_AddDerivedComponent_EnsureFoundAsParent();
        test_AddComponent_Remove();
        test_AddAndRemove_FindByBaseId();
        test_RemoveInexistentComponent();
        test_AddDuplicateComponent_EnsureError();
        test_AddOrRemoveComponentsFromWithinCallbackMethods();
    }

private:
    void test_AddComponents()
    {
        auto node = scene->createNode();
        auto a = node->addComponent<A>();
        auto b = node->addComponent<B>();
        assert(a);
        assert(b);
        assert(a->getTypeId() == A::getId());
        assert(b->getTypeId() == B::getId());
    }

    void test_AddComponent_Remove()
    {
        auto node = scene->createNode();
        node->addComponent<A>();
        node->removeComponent<A>();
        assert(node->findComponent<A>() == nullptr);
    }

    void test_AddAndRemove_FindByBaseId()
    {
        auto node = scene->createNode();
        node->addComponent<Derived>();
        node->removeComponent<Base>();
        assert(node->findComponent<Derived>() == nullptr);
        assert(node->findComponent<Base>() == nullptr);
    }

    void test_RemoveInexistentComponent()
    {
        auto node = scene->createNode();
        node->removeComponent<Derived>();
    }

    void test_AddDerivedComponent_EnsureFoundAsParent()
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
        catch (const EngineException &e)
        {
            assert(std::string(e.what()).find("already exists") != std::string::npos);
            return;
        }
        assert(false);
    }

    void test_AddOrRemoveComponentsFromWithinCallbackMethods()
    {
        auto n1 = scene->createNode();
        auto n2 = scene->createNode();
        auto n3 = scene->createNode();

        auto init1Called = false;
        auto init2Called = false;
        auto update1Called = false;
        auto update2Called = false;
        auto terminate1Called = false;
        auto terminate2Called = false;

        auto cmp = n1->addComponent<CallbackCaller>(
            [&]() // init
            {
                if (!n2->findComponent<CallbackCaller>())
                {
                    n2->addComponent<CallbackCaller>(
                        [&]() { init1Called = true; },
                        [&]() { update1Called = true; },
                        [&]() { terminate1Called = true; }
                    );
                }
            },
            [&]() // update
            {
                if (!n3->findComponent<CallbackCaller>())
                {
                    n3->addComponent<CallbackCaller>(
                        [&]() { init2Called = true; },
                        [&]() { update2Called = true; },
                        [&]() { terminate2Called = true; }
                    );
                }
            },
            [&]() // terminate
            {
                n2->removeComponent<CallbackCaller>();
                n3->removeComponent<CallbackCaller>();
            }
        );
        
        assert(init1Called);
        assert(!init2Called);
        assert(!update1Called);
        assert(!update2Called);

        scene->update();
        assert(update1Called);
        assert(init2Called);
        assert(!update2Called);

        scene->update();
        assert(update2Called);

        n1->removeComponent<CallbackCaller>();
        assert(terminate1Called);
        assert(terminate2Called);
    }
};
