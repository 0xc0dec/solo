#pragma once

#include "SoloVector3.h"
#include "SoloPlane.h"
#include "SoloMath.h"
#include <vector>


namespace solo
{
    class BoundingSphere;
    class Frustum;
    class Ray;
    class Matrix;

    class BoundingBox final
    {
    public:
        Vector3 min;
        Vector3 max;

        BoundingBox() {}
        BoundingBox(const Vector3& min, const Vector3& max);

        static auto empty() -> const BoundingBox&;

        auto getCenter() const -> Vector3;
        auto getCorners() const -> std::vector<Vector3>;

        bool intersectsBoundingBox(const BoundingBox& box) const;
        bool intersectsBoundingSphere(const BoundingSphere& sphere) const;
        bool intersectsFrustum(const Frustum& frustum) const;
        auto hitByRay(const Ray& ray) const -> float;
        auto intersectPlane(const Plane& plane) const -> PlaneIntersection;

        bool isEmpty() const;

        void mergeBoundingSphere(const BoundingSphere& sphere);
        void mergeBoundingBox(const BoundingBox& box);
    };

    inline bool BoundingBox::isEmpty() const
    {
        return Math::approxEqual(min.x, max.x, Math::smallFloat1) &&
               Math::approxEqual(min.y, max.y, Math::smallFloat1) &&
               Math::approxEqual(min.z, max.z, Math::smallFloat1);
    }
}
