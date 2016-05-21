#include "SoloPhysics.h"
#include "platform/bullet/SoloBulletPhysics.h"

using namespace solo;


auto Physics::create(Device* device, const DeviceToken& deviceToken) -> sptr<Physics>
{
    return std::make_shared<BulletPhysics>(device, deviceToken);
}


Physics::Physics(Device* device, const DeviceToken&):
    device(device)
{
}
