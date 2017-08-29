/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloPhysics.h"
#include "platform/bullet/SoloBulletPhysics.h"

using namespace solo;

auto Physics::create(Device *device, const FriendToken<Device> &deviceToken) -> sptr<Physics>
{
    return std::make_shared<bullet::Physics>(device, deviceToken);
}

Physics::Physics(Device *device, const FriendToken<Device> &):
    device(device)
{
}
