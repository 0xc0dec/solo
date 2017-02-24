/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/
#pragma once

#include "SoloVector3.h"


namespace solo
{
    class BoundingSphere;
    class BoundingBox;
    class Frustum;
    class Ray;

    enum class PlaneIntersection
    {
        Intersecting = 0,
        Front = 1,
        Back = -1
    };

    class Plane final
    {
    public:
        Plane() {}
        Plane(const Vector3 &normal, float distance);
        Plane(float normalX, float normalY, float normalZ, float distance);

        auto getNormal() const -> Vector3;
        void setNormal(const Vector3 &normal);

        auto getDistance() const -> float;
        void setDistance(float distance);

        auto getDistanceToPoint(const Vector3 &point) const -> float;

        static auto getCommonPoint(const Plane &p1, const Plane &p2, const Plane &p3) -> Vector3;

        auto intersectBoundingSphere(const BoundingSphere &sphere) const -> PlaneIntersection;
        auto intersectBoundingBox(const BoundingBox &box) const -> PlaneIntersection;
        auto intersectFrustum(const Frustum &frustum) const -> PlaneIntersection;
        auto intersectPlane(const Plane &plane) const -> PlaneIntersection;
        auto intersectRay(const Ray &ray) const -> PlaneIntersection;

        bool isParallel(const Plane &plane) const;

    private:
        Vector3 normal{0, 1, 0};
        float distance = 0;

        void normalize();
    };

    inline auto Plane::getNormal() const -> Vector3
    {
        return normal;
    }

    inline auto Plane::getDistance() const -> float
    {
        return distance;
    }
}
