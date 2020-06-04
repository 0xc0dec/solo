/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloRay.h"
#include "SoloMath.h"
#include "SoloCommon.h"

using namespace solo;

Ray::Ray(const Vector3 &origin, const Vector3 &direction):
    origin_(origin),
    direction_(direction)
{
    normalize();
}

Ray::Ray(float originX, float originY, float originZ, float dirX, float dirY, float dirZ):
    origin_(originX, originY, originZ),
    direction_(dirX, dirY, dirZ)
{
    normalize();
}

void Ray::setOrigin(const Vector3 &origin)
{
    this->origin_ = origin;
}

void Ray::setDirection(const Vector3 &direction)
{
    this->direction_ = direction;
    normalize();
}

void Ray::normalize()
{
    if (direction_.isZero())
        return;

    const auto normalizeFactor = 1.0f / direction_.length();
    if (!math::areEqual(normalizeFactor, 1.0f))
    {
        direction_.x() *= normalizeFactor;
        direction_.y() *= normalizeFactor;
        direction_.z() *= normalizeFactor;
    }
}