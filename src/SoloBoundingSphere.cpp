#include "SoloBoundingSphere.h"
#include "SoloBoundingBox.h"
#include "SoloRay.h"
#include "SoloFrustum.h"
#include <algorithm>

using namespace solo;


BoundingSphere::BoundingSphere(const Vector3& center, float radius):
    center(center),
    radius(radius)
{
}


const BoundingSphere& BoundingSphere::empty()
{
    static BoundingSphere s;
    return s;
}


bool BoundingSphere::intersectsBoundingSphere(const BoundingSphere& sphere) const
{
    auto vx = sphere.center.x - center.x;
    auto vy = sphere.center.y - center.y;
    auto vz = sphere.center.z - center.z;
    return sqrt(vx * vx + vy * vy + vz * vz) <= radius + sphere.radius;
}


bool BoundingSphere::intersectsBoundingBox(const BoundingBox& box) const
{
    auto cpX = center.x;
    auto cpY = center.y;
    auto cpZ = center.z;

    const auto& boxMin = box.min;
    const auto& boxMax = box.max;

    if (center.x < boxMin.x)
        cpX = boxMin.x;
    else if (center.x > boxMax.x)
        cpX = boxMax.x;

    if (center.y < boxMin.y)
        cpY = boxMin.y;
    else if (center.y > boxMax.y)
        cpY = boxMax.y;

    if (center.z < boxMin.z)
        cpZ = boxMin.z;
    else if (center.z > boxMax.z)
        cpZ = boxMax.z;

    cpX -= center.x;
    cpY -= center.y;
    cpZ -= center.z;

    return sqrt(cpX * cpX + cpY * cpY + cpZ * cpZ) <= radius;
}


bool BoundingSphere::intersectsFrustum(const Frustum& frustum) const
{
    return getPlaneIntersection(frustum.getNearPlane()) != PlaneIntersection::Back &&
           getPlaneIntersection(frustum.getFarPlane()) != PlaneIntersection::Back &&
           getPlaneIntersection(frustum.getLeftPlane()) != PlaneIntersection::Back &&
           getPlaneIntersection(frustum.getRightPlane()) != PlaneIntersection::Back &&
           getPlaneIntersection(frustum.getBottomPlane()) != PlaneIntersection::Back &&
           getPlaneIntersection(frustum.getTopPlane()) != PlaneIntersection::Back;
}


PlaneIntersection BoundingSphere::getPlaneIntersection(const Plane& plane) const
{
    auto distance = plane.getDistanceToPoint(center);
    if (fabsf(distance) <= radius)
        return PlaneIntersection::Intersecting;
    return distance > 0.0f ? PlaneIntersection::Front : PlaneIntersection::Back;
}


float BoundingSphere::getRayIntersection(const Ray& ray) const
{
    const auto& origin = ray.getOrigin();
    const auto& direction = ray.getDirection();

    auto vx = origin.x - center.x;
    auto vy = origin.y - center.y;
    auto vz = origin.z - center.z;
    auto d2 = vx * vx + vy * vy + vz * vz;

    // Solve the quadratic equation using the ray's and sphere's equations together.
    // Since the ray's direction is guaranteed to be 1 by the Ray, we don't need to
    // calculate and use A (A = ray.getDirection().lengthSquared()).
    auto b = 2.0f * (vx * direction.x + vy * direction.y + vz * direction.z);
    auto c = d2 - radius * radius;
    auto discriminant = b * b - 4.0f * c;

    if (discriminant < 0.0f)
        return static_cast<float>(RayIntersection::None);

    auto sqrtDisc = sqrt(discriminant);
    auto t0 = (-b - sqrtDisc) * 0.5f;
    auto t1 = (-b + sqrtDisc) * 0.5f;
    return (t0 > 0.0f && t0 < t1) ? t0 : t1;
}


void BoundingSphere::mergeBoundingSphere(const BoundingSphere& sphere)
{
    if (sphere.isEmpty())
        return;

    auto vx = center.x - sphere.center.x;
    auto vy = center.y - sphere.center.y;
    auto vz = center.z - sphere.center.z;
    auto d = sqrt(vx * vx + vy * vy + vz * vz);

    if (d <= sphere.radius - radius)
    {
        center = sphere.center;
        radius = sphere.radius;
        return;
    }
    if (d <= radius - sphere.radius)
        return;

    auto dI = 1.0f / d;
    vx *= dI;
    vy *= dI;
    vz *= dI;

    auto r = (radius + sphere.radius + d) * 0.5f;

    auto scaleFactor = r - sphere.radius;
    vx = vx * scaleFactor + sphere.center.x;
    vy = vy * scaleFactor + sphere.center.y;
    vz = vz * scaleFactor + sphere.center.z;

    center.x = vx;
    center.y = vy;
    center.z = vz;
    radius = r;
}


void BoundingSphere::mergeBoundingBox(const BoundingBox& box)
{
    if (box.isEmpty())
        return;

    const auto& min = box.min;
    const auto& max = box.max;

    auto v1x = min.x - center.x;
    auto v1y = min.y - center.y;
    auto v1z = min.z - center.z;
    auto v2x = max.x - center.x;
    auto v2y = max.y - center.y;
    auto v2z = max.z - center.z;
    auto fx = min.x;
    auto fy = min.y;
    auto fz = min.z;

    if (v2x > v1x)
        fx = max.x;
    if (v2y > v1y)
        fy = max.y;
    if (v2z > v1z)
        fz = max.z;

    v1x = center.x - fx;
    v1y = center.y - fy;
    v1z = center.z - fz;
    auto distance = sqrt(v1x * v1x + v1y * v1y + v1z * v1z);

    if (distance <= radius)
        return;

    auto dI = 1.0f / distance;
    v1x *= dI;
    v1y *= dI;
    v1z *= dI;

    auto r = (radius + distance) * 0.5f;

    v1x = v1x * r + fx;
    v1y = v1y * r + fy;
    v1z = v1z * r + fz;

    center.x = v1x;
    center.y = v1y;
    center.z = v1z;
    radius = r;
}


void BoundingSphere::transform(const Matrix& matrix)
{
    auto scale = matrix.getScale();
    auto r = radius * scale.x;
    r = std::max(r, radius * scale.y);
    r = std::max(r, radius * scale.z);
    radius = r;
    center = matrix.transformPoint(center);
}
