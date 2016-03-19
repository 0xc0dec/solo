#pragma once

#include "SoloBase.h"
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

        Plane getNearPlane() const;
        Plane getFarPlane() const;
        Plane getLeftPlane() const;
        Plane getRightPlane() const;
        Plane getTopPlane() const;
        Plane getBottomPlane() const;

        Matrix getMatrix() const;
        void setMatrix(const Matrix& m);

        std::vector<Vector3> getCorners() const;
        std::vector<Vector3> getNearCorners() const;
        std::vector<Vector3> getFarCorners() const;

        bool intersectsPoint(const Vector3& point) const;
        bool intersectsBoundingSphere(const BoundingSphere& sphere) const;
        bool intersectsBoundingBox(const BoundingBox& box) const;
        float hitByRay(const Ray& ray) const;
        PlaneIntersection intersectPlane(const Plane& plane) const;

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

    inline Plane Frustum::getNearPlane() const
    {
        return near;
    }

    inline Plane Frustum::getFarPlane() const
    {
        return far;
    }

    inline Plane Frustum::getLeftPlane() const
    {
        return left;
    }

    inline Plane Frustum::getRightPlane() const
    {
        return right;
    }

    inline Plane Frustum::getBottomPlane() const
    {
        return bottom;
    }

    inline Plane Frustum::getTopPlane() const
    {
        return top;
    }

    inline Matrix Frustum::getMatrix() const
    {
        return matrix;
    }
}
