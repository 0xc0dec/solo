/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloRay.h"
#include "SoloMath.h"
#include "SoloCommon.h"

using namespace solo;

Ray::Ray(const Vector3 &origin, const Vector3 &direction):
    origin(origin),
    direction(direction)
{
    normalize();
}

Ray::Ray(float originX, float originY, float originZ, float dirX, float dirY, float dirZ):
    origin(originX, originY, originZ),
    direction(dirX, dirY, dirZ)
{
    normalize();
}

void Ray::setOrigin(const Vector3 &origin)
{
    this->origin = origin;
}

void Ray::setDirection(const Vector3 &direction)
{
    this->direction = direction;
    normalize();
}

void Ray::normalize()
{
    if (direction.isZero())
        return;

    const auto normalizeFactor = 1.0f / direction.length();
    if (!math::areEqual(normalizeFactor, 1.0f))
    {
        direction.x() *= normalizeFactor;
        direction.y() *= normalizeFactor;
        direction.z() *= normalizeFactor;
    }
}