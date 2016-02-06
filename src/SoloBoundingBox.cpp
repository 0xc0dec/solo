#include "SoloBoundingBox.h"
#include "SoloPlane.h"
#include "SoloBoundingSphere.h"
#include "SoloMatrix.h"
#include "SoloFrustum.h"
#include "SoloRay.h"
#include <algorithm>
#include <limits>

using namespace solo;


BoundingBox::BoundingBox(const Vector3& min, const Vector3& max):
    min(min),
    max(max)
{
}


const BoundingBox& BoundingBox::empty()
{
    static BoundingBox b;
    return b;
}


std::vector<Vector3> BoundingBox::getCorners() const
{
    return
    {
        // Near face, specified counter-clockwise looking towards the origin from the positive z-axis.
        // Left-top-front
        Vector3(min.x, max.y, max.z),

        // Left-bottom-front
        Vector3(min.x, min.y, max.z),

        // Right-bottom-front
        Vector3(max.x, min.y, max.z),

        // Right-top-front
        Vector3(max.x, max.y, max.z),

        // Far face, specified counter-clockwise looking towards the origin from the negative z-axis.
        // Right-top-back.
        Vector3(max.x, max.y, min.z),

        // Right-bottom-back
        Vector3(max.x, min.y, min.z),

        // Left-bottom-back
        Vector3(min.x, min.y, min.z),

        // Left-top-back
        Vector3(min.x, max.y, min.z)
    };
}


Vector3 BoundingBox::getCenter() const
{
    auto center = max - min;
    center *= 0.5f;
    center += min;
    return center;
}


bool BoundingBox::intersectsBoundingSphere(const BoundingSphere& sphere) const
{
    return sphere.intersectsBoundingBox(*this);
}


bool BoundingBox::intersectsBoundingBox(const BoundingBox& box) const
{
    return ((min.x >= box.min.x && min.x <= box.max.x) || (box.min.x >= min.x && box.min.x <= max.x)) &&
           ((min.y >= box.min.y && min.y <= box.max.y) || (box.min.y >= min.y && box.min.y <= max.y)) &&
           ((min.z >= box.min.z && min.z <= box.max.z) || (box.min.z >= min.z && box.min.z <= max.z));
}


bool BoundingBox::intersectsFrustum(const Frustum& frustum) const
{
    return getPlaneIntersection(frustum.getNearPlane()) != PlaneIntersection::Back &&
           getPlaneIntersection(frustum.getFarPlane()) != PlaneIntersection::Back &&
           getPlaneIntersection(frustum.getLeftPlane()) != PlaneIntersection::Back &&
           getPlaneIntersection(frustum.getRightPlane()) != PlaneIntersection::Back &&
           getPlaneIntersection(frustum.getBottomPlane()) != PlaneIntersection::Back &&
           getPlaneIntersection(frustum.getTopPlane()) != PlaneIntersection::Back;
}


PlaneIntersection BoundingBox::getPlaneIntersection(const Plane& plane) const
{
    Vector3 center((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
    auto distance = plane.getDistanceToPoint(center);

    auto extentX = (max.x - min.x) * 0.5f;
    auto extentY = (max.y - min.y) * 0.5f;
    auto extentZ = (max.z - min.z) * 0.5f;

    const auto& planeNormal = plane.getNormal();
    if (fabsf(distance) <= fabsf(extentX * planeNormal.x) + fabsf(extentY * planeNormal.y) + fabsf(extentZ * planeNormal.z))
        return PlaneIntersection::Intersecting;

    return distance > 0.0f ? PlaneIntersection::Front : PlaneIntersection::Back;
}


float BoundingBox::getRayIntersection(const Ray& ray) const
{
    auto tmin = 0.0f;
    auto tmax = 0.0f;

    const auto& origin = ray.getOrigin();
    const auto& direction = ray.getDirection();

    auto div = 1.0f / direction.x;
    if (div >= 0.0f)
    {
        tmin = (min.x - origin.x) * div;
        tmax = (max.x - origin.x) * div;
    }
    else
    {
        tmin = (max.x - origin.x) * div;
        tmax = (min.x - origin.x) * div;
    }

    auto dnear = tmin;
    auto dfar = tmax;

    // Check if the ray misses the box.
    if (dnear > dfar || dfar < 0.0f)
        return static_cast<float>(RayIntersection::None);

    div = 1.0f / direction.y;
    if (div >= 0.0f)
    {
        tmin = (min.y - origin.y) * div;
        tmax = (max.y - origin.y) * div;
    }
    else
    {
        tmin = (max.y - origin.y) * div;
        tmax = (min.y - origin.y) * div;
    }

    // Update the near and far intersection distances.
    if (tmin > dnear)
        dnear = tmin;
    if (tmax < dfar)
        dfar = tmax;

    // Check if the ray misses the box.
    if (dnear > dfar || dfar < 0.0f)
        return static_cast<float>(RayIntersection::None);

    div = 1.0f / direction.z;
    if (div >= 0.0f)
    {
        tmin = (min.z - origin.z) * div;
        tmax = (max.z - origin.z) * div;
    }
    else
    {
        tmin = (max.z - origin.z) * div;
        tmax = (min.z - origin.z) * div;
    }

    // Update the near and far intersection distances.
    if (tmin > dnear)
        dnear = tmin;
    if (tmax < dfar)
        dfar = tmax;

    // Check if the ray misses the box.
    if (dnear > dfar || dfar < 0.0f)
        return static_cast<float>(RayIntersection::None);
    // The ray getIntersection the box (and since the direction of a Ray is normalized, dnear is the distance to the ray).
    return dnear;
}


void BoundingBox::mergeBoundingBox(const BoundingBox& box)
{
    min.x = std::min(min.x, box.min.x);
    min.y = std::min(min.y, box.min.y);
    min.z = std::min(min.z, box.min.z);

    max.x = std::max(max.x, box.max.x);
    max.y = std::max(max.y, box.max.y);
    max.z = std::max(max.z, box.max.z);
}


void BoundingBox::mergeBoundingSphere(const BoundingSphere& sphere)
{
    const auto& center = sphere.center;
    auto radius = sphere.radius;

    min.x = std::min(min.x, center.x - radius);
    min.y = std::min(min.y, center.y - radius);
    min.z = std::min(min.z, center.z - radius);

    max.x = std::max(max.x, center.x + radius);
    max.y = std::max(max.y, center.y + radius);
    max.z = std::max(max.z, center.z + radius);
}


static void updateMinMax(const Vector3& point, Vector3& min, Vector3& max)
{
    if (point.x < min.x)
        min.x = point.x;

    if (point.x > max.x)
        max.x = point.x;

    if (point.y < min.y)
        min.y = point.y;

    if (max.y < point.y)
        max.y = point.y;

    if (min.z > point.z)
        min.z = point.z;

    if (point.z > max.z)
        max.z = point.z;
}


void BoundingBox::transform(const Matrix& matrix)
{
    auto corners = getCorners();
	auto newMin = Vector3(std::numeric_limits<float>::max());
	auto newMax = Vector3(std::numeric_limits<float>::min());

	for (const auto& corner: corners)
    {
        auto newCorner = matrix.transformPoint(corner);
        updateMinMax(newCorner, newMin, newMax);
    }

    this->min.x = newMin.x;
    this->min.y = newMin.y;
    this->min.z = newMin.z;
    this->max.x = newMax.x;
    this->max.y = newMax.y;
    this->max.z = newMax.z;
}