#pragma once

#include "SoloBase.h"
#include "SoloVector3.h"


namespace solo
{
    class Device;
    class DeviceToken;
    class RigidBody;

    struct RaycastResult
    {
        bool anyHit = false;
        RigidBody* rigidBody = nullptr;
    };

    class Physics
    {
    public:
        static auto create(Device* device, const DeviceToken& deviceToken) -> sptr<Physics>;

        virtual ~Physics() {}
        SL_NONCOPYABLE(Physics)

        virtual void update() = 0;

        virtual void setGravity(const Vector3& gravity) = 0;

        virtual auto castRay(const Vector3& from, const Vector3& to) -> RaycastResult = 0;

    protected:
        Physics(Device* device, const DeviceToken&);

        Device* device;
    };
}