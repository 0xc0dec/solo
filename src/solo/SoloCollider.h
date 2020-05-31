/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

namespace solo
{
    class Collider
    {
    public:
        Collider(const Collider &other) = delete;
        Collider(Collider &&other) = delete;
        virtual ~Collider() = default;

        auto operator=(const Collider &other) -> Collider& = delete;
        auto operator=(Collider &&other) -> Collider& = delete;

    protected:
        Collider() = default;
    };
}