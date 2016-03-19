#pragma once

#include "SoloVector3.h"
#include "SoloPlane.h"
#include "SoloMath.h"


namespace solo
{
    class BoundingSphere final
    {
    public:
        Vector3 center;
        float radius = 0;

        static const BoundingSphere& empty();

        BoundingSphere() {}
        BoundingSphere(const Vector3& center, float radius);

        bool intersectsBoundingSphere(const BoundingSphere& sphere) const;
        bool intersectsBoundingBox(const BoundingBox& box) const;
        bool intersectsFrustum(const Frustum& frustum) const;
        float hitByRay(const Ray& ray) const;
        PlaneIntersection intersectPlane(const Plane& plane) const;

        bool isEmpty() const;

        void mergeBoundingSphere(const BoundingSphere& sphere);
        void mergeBoundingBox(const BoundingBox& box);

        void transform(const Matrix& matrix);
    };

    inline bool BoundingSphere::isEmpty() const
    {
        return Math::approxZero(radius, Math::smallFloat1);
    }
}
