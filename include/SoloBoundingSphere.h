/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

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

        BoundingSphere() {}
        BoundingSphere(const Vector3 &center, float radius);

        bool intersectsBoundingSphere(const BoundingSphere &sphere) const;
        bool intersectsBoundingBox(const BoundingBox &box) const;
        bool intersectsFrustum(const Frustum &frustum) const;
        
        auto hitByRay(const Ray &ray) const -> float;
        
        auto intersectPlane(const Plane &plane) const -> PlaneIntersection;

        bool isEmpty() const;

        void mergeBoundingSphere(const BoundingSphere &sphere);
        void mergeBoundingBox(const BoundingBox &box);
    };

    inline bool BoundingSphere::isEmpty() const
    {
        return math::isZero(radius, math::epsilon1);
    }
}
