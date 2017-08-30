/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloVector3.h"

namespace solo
{
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
