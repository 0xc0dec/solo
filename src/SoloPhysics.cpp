#include "SoloPhysics.h"
#include "SoloBulletPhysics.h"

using namespace solo;


auto Physics::create(Device* device, const DeviceToken& deviceToken) -> uptr<Physics>
{
    return std::unique_ptr<BulletPhysics>(new BulletPhysics(device, deviceToken));
}


Physics::Physics(Device* device, const DeviceToken&):
    device(device)
{
}

