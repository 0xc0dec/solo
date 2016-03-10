#pragma once

#include "SoloBase.h"
#include "SoloPlane.h"
#include "SoloMatrix.h"
#include <vector>


namespace solo
{
    struct Vector3;
    class BoundingSphere;
    class BoundingBox;
    struct Ray;

    class Frustum final
    {
    public:
        Frustum();

        const Plane& getNearPlane() const;
        const Plane& getFarPlane() const;
        const Plane& getLeftPlane() const;
        const Plane& getRightPlane() const;
        const Plane& getTopPlane() const;
        const Plane& getBottomPlane() const;

        Matrix getMatrix() const;
        void setMatrix(const Matrix& m);

        std::vector<Vector3> getCorners() const;
        std::vector<Vector3> getNearCorners() const;
        std::vector<Vector3> getFarCorners() const;

        bool intersectsPoint(const Vector3& point) const;
        bool intersectsBoundingSphere(const BoundingSphere& sphere) const;
        bool intersectsBoundingBox(const BoundingBox& box) const;
        float intersectsRay(const Ray& ray) const;
        PlaneIntersection getPlaneIntersection(const Plane& plane) const;

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

    inline const Plane& Frustum::getNearPlane() const
    {
        return near;
    }

    inline const Plane& Frustum::getFarPlane() const
    {
        return far;
    }

    inline const Plane& Frustum::getLeftPlane() const
    {
        return left;
    }

    inline const Plane& Frustum::getRightPlane() const
    {
        return right;
    }

    inline const Plane& Frustum::getBottomPlane() const
    {
        return bottom;
    }

    inline const Plane& Frustum::getTopPlane() const
    {
        return top;
    }

    inline Matrix Frustum::getMatrix() const
    {
        return matrix;
    }
}
