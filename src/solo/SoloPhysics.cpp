/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloPhysics.h"
#include "bullet/SoloBulletPhysics.h"

using namespace solo;

auto Physics::fromDevice(Device *device) -> sptr<Physics>
{
    return std::make_shared<BulletPhysics>(device);
}

Physics::Physics(Device *device):
    device_(device)
{
}
