#pragma once

#include "SoloVector3.h"


namespace solo
{
    class Frustum;
    struct Plane;
    class BoundingSphere;
    class BoundingBox;
    struct Matrix;

    enum struct RayIntersection
    {
        None = -1
    };

    struct Ray final
    {
        Ray() {}
        Ray(const Vector3& origin, const Vector3& direction);
        Ray(float originX, float originY, float originZ, float dirX, float dirY, float dirZ);

        Vector3 getOrigin() const;
        void setOrigin(const Vector3& origin);
        void setOrigin(float x, float y, float z);

        Vector3 getDirection() const;
        void setDirection(const Vector3& direction);
        void setDirection(float x, float y, float z);

        float getIntersection(const BoundingSphere& sphere) const;
        float getIntersection(const BoundingBox& box) const;
        float getIntersection(const Frustum& frustum) const;
        float getIntersection(const Plane& plane) const;

        void transform(const Matrix& matrix);

        Ray& operator*=(const Matrix& matrix);

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

    inline Ray& Ray::operator*=(const Matrix& matrix)
    {
        transform(matrix);
        return *this;
    }

    inline Ray operator*(const Matrix& matrix, const Ray& ray)
    {
        auto r(const_cast<Ray&>(ray));
        r.transform(matrix);
        return r;
    }
}
