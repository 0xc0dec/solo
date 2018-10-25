/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloBulletBoxCollider.h"
#include "SoloVector3.h"
#include "SoloBulletCommon.h"

using namespace solo;

BulletBoxCollider::BulletBoxCollider(const Vector3 &halfExtents)
{
    shape_ = std::make_unique<btBoxShape>(SL_TOBTVEC3(halfExtents));
}
