#pragma once

#include "SoloBase.h"
#include "SoloVector3.h"
#include "SoloPlane.h"
#include "SoloMath.h"

namespace solo
{
    class BoundingSphere;
    class Frustum;
    struct Ray;
    struct Matrix;

    class BoundingBox
    {
    public:
        Vector3 min;
        Vector3 max;

        BoundingBox() {}
        BoundingBox(const Vector3 &min, const Vector3 &max);

        static const BoundingBox &empty();

        Vector3 getCenter() const;
        std::vector<Vector3> getCorners() const;

        bool intersectsBoundingBox(const BoundingBox &box) const;
        bool intersectsBoundingSphere(const BoundingSphere &sphere) const;
        bool intersectsFrustum(const Frustum &frustum) const;
        float getRayIntersection(const Ray &ray) const;
        PlaneIntersection getPlaneIntersection(const Plane &plane) const;

        bool isEmpty() const;

        void mergeBoundingSphere(const BoundingSphere &sphere);
        void mergeBoundingBox(const BoundingBox &box);

        void transform(const Matrix &matrix);

        BoundingBox &operator*=(const Matrix &matrix);
    };


    inline bool BoundingBox::isEmpty() const
    {
        return Math::approxEqual(min.x, max.x) && Math::approxEqual(min.y, max.y) && Math::approxEqual(min.z, max.z);
    }

    inline BoundingBox &BoundingBox::operator*=(const Matrix &matrix)
    {
        transform(matrix);
        return *this;
    }

    inline BoundingBox operator*(const Matrix &matrix, const BoundingBox &box)
    {
        auto b(const_cast<BoundingBox &>(box));
        b.transform(matrix);
        return b;
    }
}
