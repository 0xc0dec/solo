/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloVector3.h"


namespace solo
{
    class Plane;
    class BoundingSphere;
    class BoundingBox;
    class Frustum;

    class Ray final
    {
    public:
        Ray() {}
        Ray(const Vector3 &origin, const Vector3 &direction);
        Ray(float originX, float originY, float originZ, float dirX, float dirY, float dirZ);

        auto getOrigin() const -> Vector3;
        void setOrigin(const Vector3 &origin);

        auto getDirection() const -> Vector3;
        void setDirection(const Vector3 &direction);

        // These methods return -1 if there's no intersection
        auto hitBoundingSphere(const BoundingSphere &sphere) const -> float;
        auto hitBoundingBox(const BoundingBox &box) const -> float;
        auto hitFrustum(const Frustum &frustum) const -> float;
        auto hitPlane(const Plane &plane) const -> float;

    private:
        Vector3 origin{0, 0, 0};
        Vector3 direction{0, 0, 1};
      
        void normalize();
    };

    inline auto Ray::getOrigin() const -> Vector3
    {
        return origin;
    }

    inline auto Ray::getDirection() const -> Vector3
    {
        return direction;
    }
}
