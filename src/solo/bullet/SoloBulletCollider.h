/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include <btBulletCollisionCommon.h>

namespace solo {
    class BulletCollider {
    public:
        BulletCollider(const BulletCollider &other) = delete;
        BulletCollider(BulletCollider &&other) = delete;
        virtual ~BulletCollider() = default;

        auto operator=(const BulletCollider &other) -> BulletCollider & = delete;
        auto operator=(BulletCollider &&other) -> BulletCollider & = delete;

        virtual auto shape() -> btCollisionShape * = 0;

    protected:
        BulletCollider() = default;
    };
}