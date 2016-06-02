#pragma once

#include "TestBase.h"


class A final: public ComponentBase<A>
{
public:
    explicit A(const Node& node): ComponentBase<A>(node)
    {
    }
};


class B final: public ComponentBase<B>
{
public:
    explicit B(const Node& node): ComponentBase<B>(node)
    {
    }
};


class C final: public ComponentBase<C>
{
public:
    Component* addedCmp = nullptr;
    Component* removedCmp = nullptr;

    explicit C(const Node& node) : ComponentBase<C>(node)
    {
    }

    void onComponentAdded(Component* cmp) override final
    {
        addedCmp = cmp;
    }

    void onComponentRemoved(Component* cmp) override final
    {
        removedCmp = cmp;
    }
};


class CallbackCaller final: public ComponentBase<CallbackCaller>
{
public:
    explicit CallbackCaller(const Node& node, std::function<void()> initAction, std::function<void()> updateAction, std::function<void()> terminateAction):
        ComponentBase<CallbackCaller>(node),
        initAction(initAction),
        updateAction(updateAction),
        terminateAction(terminateAction)
    {
    }

    void init() override final
    {
        if (initAction)
            initAction();
    }

    void update() override final
    {
        if (updateAction)
            updateAction();
    }

    void terminate() override final
    {
        if (terminateAction)
            terminateAction();
    }

private:
    std::function<void()> initAction;
    std::function<void()> updateAction;
    std::function<void()> terminateAction;
};


class Base: public ComponentBase<Base>
{
public:
    explicit Base(const Node& node): ComponentBase<Base>(node)
    {
    }
};


class Derived final: public Base
{
public:
    explicit Derived(const Node& node): Base(node)
    {
    }
};


class Components_Test final: public TestBase
{
public:
    Components_Test(Device *device): TestBase(device)
    {
    }

    void run() override final
    {
        test_AddComponents();
        test_AddDerivedComponent_EnsureFoundAsParent();
        test_AddComponent_Remove();
        test_AddAndRemove_FindByBaseId();
        test_RemoveInexistentComponent();
        test_AddDuplicateComponent_EnsureError();
        test_AddOrRemoveComponentsFromWithinCallbackMethods();
        test_AddOrRemoveComponents_EnsureAddedRemovedEventsCalled();
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

    void test_AddOrRemoveComponents_EnsureAddedRemovedEventsCalled()
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
                n2->removeComponent<CallbackCaller>();
            },
            [&]() // terminate
            {
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
