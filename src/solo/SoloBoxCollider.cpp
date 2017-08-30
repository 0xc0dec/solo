/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloBoxCollider.h"
#include "bullet/SoloBulletBoxCollider.h"

using namespace solo;

auto BoxCollider::create(const Vector3 &size) -> sptr<BoxCollider>
{
    return std::make_shared<bullet::BoxCollider>(size);
}
