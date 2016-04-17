#pragma once

#include "SoloPhysics.h"

namespace solo
{
    class BulletPhysics: public Physics
    {
    public:
        

    private:
        friend class Physics;

        BulletPhysics(Device* device, const DeviceToken& deviceToken);
    };
}
