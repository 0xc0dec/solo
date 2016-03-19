#pragma once

#include "SoloVector3.h"


namespace solo
{
    class Plane;
    class BoundingSphere;
    class BoundingBox;
    class Frustum;
    class Matrix;

    class Ray final
    {
    public:
        Ray() {}
        Ray(const Vector3& origin, const Vector3& direction);
        Ray(float originX, float originY, float originZ, float dirX, float dirY, float dirZ);

        Vector3 getOrigin() const;
        void setOrigin(const Vector3& origin);

        Vector3 getDirection() const;
        void setDirection(const Vector3& direction);

        // These methods return -1 if there's no intersection
        float hitBoundingSphere(const BoundingSphere& sphere) const;
        float hitBoundingBox(const BoundingBox& box) const;
        float hitFrustum(const Frustum& frustum) const;
        float hitPlane(const Plane& plane) const;

        void transform(const Matrix& matrix);

    private:
        void normalize();

        Vector3 origin { 0, 0, 0 };
        Vector3 direction { 0, 0, 1 };
    };

    inline Vector3 Ray::getOrigin() const
    {
        return origin;
    }

    inline Vector3 Ray::getDirection() const
    {
        return direction;
    }
}
