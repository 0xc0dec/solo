#pragma once

#include "SoloBase.h"
#include "SoloVector3.h"
#include <vector>


namespace solo
{
    class Device;
    class DeviceToken;
    class RigidBody;

    struct RaycastResult
    {
        RigidBody* body;
        Vector3 point;
        Vector3 normal;

        RaycastResult(RigidBody* body = nullptr, const Vector3& point = Vector3::zero(), const Vector3& normal = Vector3::zero()):
            body(body), point(point), normal(normal)
        {
        }
    };

    class Physics
    {
    public:
        static auto create(Device* device, const DeviceToken& deviceToken) -> sptr<Physics>;

        virtual ~Physics() {}
        SL_NONCOPYABLE(Physics)

        virtual void update() = 0;

        virtual void setGravity(const Vector3& gravity) = 0;

        virtual auto castRay(const Vector3& from, const Vector3& to, bool onlyFirstHit) -> std::vector<RaycastResult> = 0;

    protected:
        Physics(Device* device, const DeviceToken&);

        Device* device;
    };
}