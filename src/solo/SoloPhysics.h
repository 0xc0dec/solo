/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"
#include "math/SoloVector3.h"

namespace solo {
    class Device;
    class RigidBody;

    class RayTestResult {
    public:
        RigidBody *body;
        Vector3 point;
        Vector3 normal;

        explicit RayTestResult(RigidBody *body = nullptr, const Vector3 &point = Vector3(), const Vector3 &normal = Vector3()):
            body(body), point(point), normal(normal) {
        }
    };

    class Physics {
    public:
        static auto fromDevice(Device *device) -> sptr<Physics>;

        Physics() = delete;
        Physics(const Physics &other) = delete;
        Physics(Physics &&other) = delete;
        virtual ~Physics() = default;

        auto operator=(const Physics &other) -> Physics & = delete;
        auto operator=(Physics &&other) -> Physics & = delete;

        virtual void update() = 0;

        virtual void setGravity(const Vector3 &gravity) = 0;

        virtual auto rayTestFirst(const Vector3 &from, const Vector3 &to) -> RayTestResult = 0;
        virtual auto rayTestAll(const Vector3 &from, const Vector3 &to) -> vec<RayTestResult> = 0;

    protected:
        Device *device_ = nullptr;

        Physics(Device *device);
    };
}