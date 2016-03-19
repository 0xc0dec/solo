#include "SoloRay.h"
#include "SoloMath.h"
#include "SoloBoundingSphere.h"
#include "SoloBoundingBox.h"
#include "SoloMatrix.h"
#include "SoloFrustum.h"
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


float Ray::hitBoundingSphere(const BoundingSphere& sphere) const
{
    return sphere.hitByRay(*this);
}


float Ray::hitBoundingBox(const BoundingBox& box) const
{
    return box.hitByRay(*this);
}


float Ray::hitFrustum(const Frustum& frustum) const
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


float Ray::hitPlane(const Plane& plane) const
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


void Ray::transform(const Matrix& matrix)
{
    origin = matrix.transformPoint(origin);
    direction = matrix.transformDirection(direction);
    direction.normalize();
}


void Ray::normalize()
{
    SL_DEBUG_THROW_IF(direction.isZero(), InvalidOperationException, "Invalid ray object: the ray's direction must be non-zero");

    // Normalize the ray's direction vector
    auto normalizeFactor = 1.0f / sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (!Math::approxEqual(normalizeFactor, 1.0f, Math::smallFloat1))
    {
        direction.x *= normalizeFactor;
        direction.y *= normalizeFactor;
        direction.z *= normalizeFactor;
    }
}