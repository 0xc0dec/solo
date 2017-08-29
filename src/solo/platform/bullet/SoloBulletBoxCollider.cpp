/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloBulletBoxCollider.h"
#include "SoloVector3.h"
#include "SoloBulletCommon.h"

using namespace solo;

bullet::BoxCollider::BoxCollider(const Vector3 &halfExtents)
{
    shape = std::make_unique<btBoxShape>(SL_TOBTVEC3(halfExtents));
}
