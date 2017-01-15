/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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
        void normalize();

        Vector3 normal{0, 1, 0};
        float distance = 0;
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
