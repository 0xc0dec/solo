#pragma once

#include "SoloPlane.h"
#include "SoloMatrix.h"
#include <vector>


namespace solo
{
    class Vector3;
    class BoundingSphere;
    class BoundingBox;
    class Ray;

    class Frustum final
    {
    public:
        Frustum();

        auto getNearPlane() const -> Plane;
        auto getFarPlane() const -> Plane;
        auto getLeftPlane() const -> Plane;
        auto getRightPlane() const -> Plane;
        auto getTopPlane() const -> Plane;
        auto getBottomPlane() const -> Plane;

        auto getMatrix() const -> Matrix;
        void setMatrix(const Matrix& m);

        auto getCorners() const -> std::vector<Vector3>;
        auto getNearCorners() const -> std::vector<Vector3>;
        auto getFarCorners() const -> std::vector<Vector3>;

        bool intersectsPoint(const Vector3& point) const;
        bool intersectsBoundingSphere(const BoundingSphere& sphere) const;
        bool intersectsBoundingBox(const BoundingBox& box) const;
        auto hitByRay(const Ray& ray) const -> float;
        auto intersectPlane(const Plane& plane) const -> PlaneIntersection;

    private:
        void updatePlanes();

        Plane near;
        Plane far;
        Plane bottom;
        Plane top;
        Plane left;
        Plane right;
        Matrix matrix;
    };

    inline auto Frustum::getNearPlane() const -> Plane
    {
        return near;
    }

    inline auto Frustum::getFarPlane() const -> Plane
    {
        return far;
    }

    inline auto Frustum::getLeftPlane() const -> Plane
    {
        return left;
    }

    inline auto Frustum::getRightPlane() const -> Plane
    {
        return right;
    }

    inline auto Frustum::getBottomPlane() const -> Plane
    {
        return bottom;
    }

    inline auto Frustum::getTopPlane() const -> Plane
    {
        return top;
    }

    inline auto Frustum::getMatrix() const -> Matrix
    {
        return matrix;
    }
}
