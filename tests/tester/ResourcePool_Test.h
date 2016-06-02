#pragma once

#include "TestBase.h"


class ResourcePool_Test final: TestBase
{
public:
    explicit ResourcePool_Test(Device *device): TestBase(device)
    {
    }

    void run() override final
    {
        test_ReserveRelease_HandleCount();
        test_HandleReuse();
        test_ReserveRelease_DataPersistence();
    }

private:
    void test_ReserveRelease_DataPersistence()
    {
        ResourcePool<int, 10> pool;

        auto h1 = pool.reserveHandle();
        auto h2 = pool.reserveHandle();
        auto h3 = pool.reserveHandle();
        auto h4 = pool.reserveHandle();
        auto h5 = pool.reserveHandle();

        pool.getData(h1) = 100;
        pool.getData(h2) = 200;
        pool.getData(h3) = 300;
        pool.getData(h4) = 400;
        pool.getData(h5) = 500;

        pool.releaseHandle(h1);
        pool.releaseHandle(h2);
        pool.releaseHandle(h5);
        pool.releaseHandle(h4);

        h1 = pool.reserveHandle();
        h2 = pool.reserveHandle();
        h4 = pool.reserveHandle();
        h5 = pool.reserveHandle();

        assert(pool.getData(h3) == 300);
    }

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