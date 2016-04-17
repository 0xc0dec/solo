#pragma once

#include "SoloBase.h"


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

    protected:
        Physics(Device* device, const DeviceToken&);

        Device* device;
    };
}