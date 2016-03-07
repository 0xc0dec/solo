#pragma once

#include "TestBase.h"


class ResourcePool_Test: TestBase
{
public:
    explicit ResourcePool_Test(Device *device): TestBase(device)
    {
    }

    virtual void run() override final
    {
        test_ReserveRelease_HandleCount();
        test_HandleReuse();
    }

private:
    void test_ReserveRelease_HandleCount()
    {
        ResourcePool<int, 5> pool;
        assert(pool.getHandleCount() == 0);

        auto h1 = pool.reserveHandle();
        auto h2 = pool.reserveHandle();
        auto h3 = pool.reserveHandle();
        assert(pool.getHandleCount() == 3);

        pool.releaseHandle(h2);
        assert(pool.getHandleCount() == 2);

        pool.releaseHandle(h1);
        pool.releaseHandle(h3);
        assert(pool.getHandleCount() == 0);
    }

    void test_HandleReuse()
    {
        ResourcePool<int, 5> pool;

        auto h1 = pool.reserveHandle();
        auto h2 = pool.reserveHandle();
        auto h3 = pool.reserveHandle();
        assert(h1 == 0);
        assert(h2 == 1);
        assert(h3 == 2);

        pool.releaseHandle(h3);
        pool.releaseHandle(h1);
        h1 = pool.reserveHandle();
        h3 = pool.reserveHandle();
        assert(h1 == 0);
        assert(h3 == 2);
    }
};