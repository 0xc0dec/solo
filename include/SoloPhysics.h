#pragma once

#include "SoloBase.h"
#include "SoloVector3.h"


namespace solo
{
    class Device;
    class DeviceToken;

    class Physics
    {
    public:
        static auto create(Device* device, const DeviceToken& deviceToken) -> uptr<Physics>;

        virtual ~Physics() {}
        SL_NONCOPYABLE(Physics)

        virtual void update() = 0;

        virtual void setGravity(const Vector3& gravity) = 0;

    protected:
        Physics(Device* device, const DeviceToken&);

        Device* device;
    };
}