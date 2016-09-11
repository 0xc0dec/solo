/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloRay.h"
#include "SoloMath.h"
#include "SoloBoundingSphere.h"
#include "SoloBoundingBox.h"
#include "SoloFrustum.h"
#include "SoloCommon.h"
#include <algorithm>

using namespace solo;


Ray::Ray(const Vector3& origin, const Vector3& direction):
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


void Ray::setOrigin(const Vector3& origin)
{
    this->origin = origin;
}


void Ray::setDirection(const Vector3& direction)
{
    this->direction = direction;
    normalize();
}


auto Ray::hitBoundingSphere(const BoundingSphere& sphere) const -> float
{
    return sphere.hitByRay(*this);
}


auto Ray::hitBoundingBox(const BoundingBox& box) const -> float
{
    return box.hitByRay(*this);
}


auto Ray::hitFrustum(const Frustum& frustum) const -> float
{
    auto n = frustum.getNearPlane();
    auto nD = hitPlane(n);
    auto nOD = n.getDistanceToPoint(origin);

    auto f = frustum.getFarPlane();
    auto fD = hitPlane(f);
    auto fOD = f.getDistanceToPoint(origin);

    auto l = frustum.getLeftPlane();
    auto lD = hitPlane(l);
    auto lOD = l.getDistanceToPoint(origin);

    auto r = frustum.getRightPlane();
    auto rD = hitPlane(r);
    auto rOD = r.getDistanceToPoint(origin);

    auto b = frustum.getBottomPlane();
    auto bD = hitPlane(b);
    auto bOD = b.getDistanceToPoint(origin);

    auto t = frustum.getTopPlane();
    auto tD = hitPlane(t);
    auto tOD = t.getDistanceToPoint(origin);

    // If the ray's origin is in the negative half-space of one of the frustum's planes
    // and it does not intersect that same plane, then it does not intersect the frustum.
    if ((nOD < 0.0f && nD < 0.0f) || (fOD < 0.0f && fD < 0.0f) ||
        (lOD < 0.0f && lD < 0.0f) || (rOD < 0.0f && rD < 0.0f) ||
        (bOD < 0.0f && bD < 0.0f) || (tOD < 0.0f && tD < 0.0f))
        return -1;

    // Otherwise, the intersection distance is the minimum positive intersection distance.
    auto d = (nD > 0.0f) ? nD : 0.0f;
    d = (fD > 0.0f) ? ((d == 0.0f) ? fD : std::min(fD, d)) : d;
    d = (lD > 0.0f) ? ((d == 0.0f) ? lD : std::min(lD, d)) : d;
    d = (rD > 0.0f) ? ((d == 0.0f) ? rD : std::min(rD, d)) : d;
    d = (tD > 0.0f) ? ((d == 0.0f) ? bD : std::min(bD, d)) : d;
    d = (bD > 0.0f) ? ((d == 0.0f) ? tD : std::min(tD, d)) : d;

    return d;
}


auto Ray::hitPlane(const Plane& plane) const -> float
{
    const auto& normal = plane.getNormal();

    auto alpha = normal.dot(origin) + plane.getDistance();
    if (Math::approxZero(alpha, Math::smallFloat1))
        return 0.0f;

    auto dot = normal.dot(direction);

    if (Math::approxZero(dot, Math::smallFloat1))
        return -1;

    // Calculate the distance along the ray's direction vector to the point where
    // the ray getIntersection the plane (if it is negative the plane is behind the ray).
    auto d = -alpha / dot;
    return d < 0.0f ? -1 : d;
}


void Ray::normalize()
{
    SL_ERR_IF(direction.isZero())

    // Normalize the ray's direction vector
    auto normalizeFactor = 1.0f / sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (!Math::approxEqual(normalizeFactor, 1.0f, Math::smallFloat1))
    {
        direction.x *= normalizeFactor;
        direction.y *= normalizeFactor;
        direction.z *= normalizeFactor;
    }
}