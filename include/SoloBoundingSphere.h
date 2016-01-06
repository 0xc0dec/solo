#pragma once

#include "SoloVector3.h"
#include "SoloPlane.h"

namespace solo
{
    class BoundingSphere
    {
    public:
        Vector3 center;
        float radius = 0;

        BoundingSphere() {}
        BoundingSphere(const Vector3 &center, float radius);

        static const BoundingSphere &empty();

        bool intersectsBoundingSphere(const BoundingSphere &sphere) const;
        bool intersectsBoundingBox(const BoundingBox &box) const;
        bool intersectsFrustum(const Frustum &frustum) const;
        float getRayIntersection(const Ray &ray) const;
        PlaneIntersection getPlaneIntersection(const Plane &plane) const;

        bool isEmpty() const;

        void mergeBoundingSphere(const BoundingSphere &sphere);
        void mergeBoundingBox(const BoundingBox &box);

        void transform(const Matrix &matrix);

        inline BoundingSphere &operator*=(const Matrix &matrix);
    };


    inline bool BoundingSphere::isEmpty() const
    {
        return radius == 0.0f;
    }

    inline BoundingSphere &BoundingSphere::operator*=(const Matrix &matrix)
    {
        transform(matrix);
        return *this;
    }

    inline BoundingSphere operator*(const Matrix &matrix, const BoundingSphere &sphere)
    {
        auto s(const_cast<BoundingSphere &>(sphere));
        s.transform(matrix);
        return s;
    }
}
