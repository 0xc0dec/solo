#pragma once

#include "SoloVector3.h"


namespace solo
{
    class BoundingSphere;
    class BoundingBox;
    class Frustum;
    class Ray;
    class Matrix;

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
        Plane(const Vector3& normal, float distance);
        Plane(float normalX, float normalY, float normalZ, float distance);

        Vector3 getNormal() const;
        void setNormal(const Vector3& normal);

        float getDistance() const;
        void setDistance(float distance);

        float getDistanceToPoint(const Vector3& point) const;

        static Vector3 getCommonPoint(const Plane& p1, const Plane& p2, const Plane& p3);

        PlaneIntersection intersectBoundingSphere(const BoundingSphere& sphere) const;
        PlaneIntersection intersectBoundingBox(const BoundingBox& box) const;
        PlaneIntersection intersectFrustum(const Frustum& frustum) const;
        PlaneIntersection intersectPlane(const Plane& plane) const;
        PlaneIntersection intersectRay(const Ray& ray) const;

        bool isParallel(const Plane& plane) const;

        void transform(const Matrix& matrix);

    private:
        void normalize();

        Vector3 normal { 0, 1, 0 };
        float distance = 0;
    };

    inline Vector3 Plane::getNormal() const
    {
        return normal;
    }

    inline float Plane::getDistance() const
    {
        return distance;
    }
}
