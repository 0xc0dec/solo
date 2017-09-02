/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVector3.h"
#include <vector>

namespace solo
{
    class Device;
    class RigidBody;

    class RaycastResult
    {
    public:
        RigidBody *body;
        Vector3 point;
        Vector3 normal;

        explicit RaycastResult(RigidBody *body = nullptr, const Vector3 &point = Vector3(), const Vector3 &normal = Vector3()):
            body(body), point(point), normal(normal)
        {
        }
    };

    class Physics
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Physics)

        static auto create(Device *device, const FriendToken<Device> &deviceToken) -> sptr<Physics>;

        virtual ~Physics() {}

        virtual void update() = 0;

        virtual void setGravity(const Vector3 &gravity) = 0;

        virtual auto castRay(const Vector3 &from, const Vector3 &to) -> RaycastResult = 0;
        virtual auto castRayAll(const Vector3 &from, const Vector3 &to) -> std::vector<RaycastResult> = 0;

    protected:
        Device *device = nullptr;
      
        Physics(Device *device, const FriendToken<Device> &);
    };
}