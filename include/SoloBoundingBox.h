/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

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
        BoundingBox(const Vector3 &min, const Vector3 &max);

        auto getCenter() const -> Vector3;
        auto getCorners() const -> std::vector<Vector3>;

        bool intersectsBoundingBox(const BoundingBox &box) const;
        bool intersectsBoundingSphere(const BoundingSphere &sphere) const;
        bool intersectsFrustum(const Frustum &frustum) const;
        
        auto hitByRay(const Ray &ray) const -> float;
        
        auto intersectPlane(const Plane &plane) const -> PlaneIntersection;

        bool isEmpty() const;

        void mergeBoundingSphere(const BoundingSphere &sphere);
        void mergeBoundingBox(const BoundingBox &box);
    };

    inline bool BoundingBox::isEmpty() const
    {
        return math::areEqual(min.x, max.x, math::epsilon1) &&
               math::areEqual(min.y, max.y, math::epsilon1) &&
               math::areEqual(min.z, max.z, math::epsilon1);
    }
}
