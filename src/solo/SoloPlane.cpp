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

#include "SoloPlane.h"
#include "SoloMath.h"
#include "SoloBoundingSphere.h"
#include "SoloBoundingBox.h"
#include "SoloFrustum.h"
#include "SoloRay.h"

using namespace solo;


Plane::Plane(const Vector3 &normal, float distance):
    normal(normal),
    distance(distance)
{
}


Plane::Plane(float normalX, float normalY, float normalZ, float distance):
    normal(normalX, normalY, normalZ),
    distance(distance)
{
}


void Plane::setNormal(const Vector3 &normal)
{
    this->normal = normal;
    normalize();
}


void Plane::setDistance(float distance)
{
    this->distance = distance;
}


auto Plane::getDistanceToPoint(const Vector3 &point) const -> float
{
    return normal.x * point.x + normal.y * point.y + normal.z * point.z + distance;
}


auto Plane::getCommonPoint(const Plane &p1, const Plane &p2, const Plane &p3) -> Vector3
{
    Vector3 result;

    // The planes' normals must be all normalized (which we guarantee in the Plane class).
    // Calculate the determinant of the matrix (i.e | n1 n2 n3 |).
    auto det =
    p1.normal.x * (p2.normal.y * p3.normal.z - p2.normal.z * p3.normal.y) -
    p2.normal.x * (p1.normal.y * p3.normal.z - p1.normal.z * p3.normal.y) +
    p3.normal.x * (p1.normal.y * p2.normal.z - p1.normal.z * p2.normal.y);

    // If the determinant is zero, then the planes do not all intersect
    if (math::isZero(det, math::epsilon1))
        return result;

    // Create 3 points, one on each plane
    // (we just pick the point on the plane directly along its normal from the origin)
    auto p1x = -p1.normal.x * p1.distance;
    auto p1y = -p1.normal.y * p1.distance;
    auto p1z = -p1.normal.z * p1.distance;
    auto p2x = -p2.normal.x * p2.distance;
    auto p2y = -p2.normal.y * p2.distance;
    auto p2z = -p2.normal.z * p2.distance;
    auto p3x = -p3.normal.x * p3.distance;
    auto p3y = -p3.normal.y * p3.distance;
    auto p3z = -p3.normal.z * p3.distance;

    // Calculate the cross products of the normals
    auto c1x = p2.normal.y * p3.normal.z - p2.normal.z * p3.normal.y;
    auto c1y = p2.normal.z * p3.normal.x - p2.normal.x * p3.normal.z;
    auto c1z = p2.normal.x * p3.normal.y - p2.normal.y * p3.normal.x;
    auto c2x = p3.normal.y * p1.normal.z - p3.normal.z * p1.normal.y;
    auto c2y = p3.normal.z * p1.normal.x - p3.normal.x * p1.normal.z;
    auto c2z = p3.normal.x * p1.normal.y - p3.normal.y * p1.normal.x;
    auto c3x = p1.normal.y * p2.normal.z - p1.normal.z * p2.normal.y;
    auto c3y = p1.normal.z * p2.normal.x - p1.normal.x * p2.normal.z;
    auto c3z = p1.normal.x * p2.normal.y - p1.normal.y * p2.normal.x;

    // Calculate the point of intersection using the formula:
    // x = (| n1 n2 n3 |)^-1 * [(x1 * n1)(n2 x n3) + (x2 * n2)(n3 x n1) + (x3 * n3)(n1 x n2)]
    auto s1 = p1x * p1.normal.x + p1y * p1.normal.y + p1z * p1.normal.z;
    auto s2 = p2x * p2.normal.x + p2y * p2.normal.y + p2z * p2.normal.z;
    auto s3 = p3x * p3.normal.x + p3y * p3.normal.y + p3z * p3.normal.z;
    auto invDet = 1.0f / det;
    result.x = (s1 * c1x + s2 * c2x + s3 * c3x) * invDet;
    result.y = (s1 * c1y + s2 * c2y + s3 * c3y) * invDet;
    result.z = (s1 * c1z + s2 * c2z + s3 * c3z) * invDet;
    return result;
}


auto Plane::intersectBoundingSphere(const BoundingSphere &sphere) const -> PlaneIntersection
{
    return sphere.intersectPlane(*this);
}


auto Plane::intersectBoundingBox(const BoundingBox &box) const -> PlaneIntersection
{
    return box.intersectPlane(*this);
}


auto Plane::intersectFrustum(const Frustum &frustum) const -> PlaneIntersection
{
    auto corners = frustum.getCorners();

    // Calculate the distances from all of the corners to the plane.
    // If all of the distances are positive, then the frustum is in the
    // positive half-space of this plane; if all the distances are negative,
    // then the frustum is in the negative half-space of this plane; if some of
    // the distances are positive and some are negative, the frustum getIntersection.
    auto d = getDistanceToPoint(corners[0]);
    if (d > 0.0f)
    {
        if (getDistanceToPoint(corners[1]) <= 0.0f ||
                getDistanceToPoint(corners[2]) <= 0.0f ||
                getDistanceToPoint(corners[3]) <= 0.0f ||
                getDistanceToPoint(corners[4]) <= 0.0f ||
                getDistanceToPoint(corners[5]) <= 0.0f ||
                getDistanceToPoint(corners[6]) <= 0.0f ||
                getDistanceToPoint(corners[7]) <= 0.0f)
            return PlaneIntersection::Intersecting;

        return PlaneIntersection::Front;
    }
    if (d < 0.0f)
    {
        if (getDistanceToPoint(corners[1]) >= 0.0f ||
                getDistanceToPoint(corners[2]) >= 0.0f ||
                getDistanceToPoint(corners[3]) >= 0.0f ||
                getDistanceToPoint(corners[4]) >= 0.0f ||
                getDistanceToPoint(corners[5]) >= 0.0f ||
                getDistanceToPoint(corners[6]) >= 0.0f ||
                getDistanceToPoint(corners[7]) >= 0.0f)
            return PlaneIntersection::Intersecting;

        return PlaneIntersection::Back;
    }

    return PlaneIntersection::Intersecting;
}


auto Plane::intersectPlane(const Plane &plane) const -> PlaneIntersection
{
    if ((math::areEqual(normal.x, plane.normal.x, math::epsilon1) &&
            math::areEqual(normal.y, plane.normal.y, math::epsilon1) &&
            math::areEqual(normal.z, plane.normal.z, math::epsilon1)) || !isParallel(plane))
        return PlaneIntersection::Intersecting;

    // Calculate the point where the given plane's normal vector intersects the given plane
    Vector3 point(plane.normal.x * -plane.distance, plane.normal.y * -plane.distance, plane.normal.z * -plane.distance);

    // Calculate whether the given plane is in the positive or negative half-space of this plane
    // (corresponds directly to the sign of the distance from the point calculated above to this plane).
    return getDistanceToPoint(point) > 0.0f ? PlaneIntersection::Front : PlaneIntersection::Back;
}


auto Plane::intersectRay(const Ray &ray) const -> PlaneIntersection
{
    auto d = getDistanceToPoint(ray.getOrigin());

    // If the origin of the ray lies in the plane, then it getIntersection.
    if (math::isZero(d, math::epsilon1))
        return PlaneIntersection::Intersecting;

    auto rayDirection = ray.getDirection();
    // If the dot product of this plane's normal and the ray's direction is positive, and
    // if the distance from this plane to the ray's origin is negative -> intersection, OR
    // if the dot product of this plane's normal and the ray's direction is negative, and
    // if the distance from this plane to the ray's origin is positive -> intersection.
    if (normal.x * rayDirection.x + normal.y * rayDirection.y + normal.z * rayDirection.z > 0.0f)
        return d < 0.0f ? PlaneIntersection::Intersecting : PlaneIntersection::Front;
    return d > 0.0f ? PlaneIntersection::Intersecting : PlaneIntersection::Back;
}


bool Plane::isParallel(const Plane &plane) const
{
    return math::areEqual(normal.y * plane.normal.z, normal.z * plane.normal.y, math::epsilon1) &&
           math::areEqual(normal.z * plane.normal.x, normal.x * plane.normal.z, math::epsilon1) &&
           math::areEqual(normal.x * plane.normal.y, normal.y * plane.normal.x, math::epsilon1);
}


void Plane::normalize()
{
    if (normal.isZero())
        return;

    // Normalize the plane's normal
    auto normalizeFactor = 1.0f / sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

    if (!math::areEqual(normalizeFactor, 1.0f, math::epsilon1))
    {
        normal.x *= normalizeFactor;
        normal.y *= normalizeFactor;
        normal.z *= normalizeFactor;
        distance *= normalizeFactor;
    }
}
