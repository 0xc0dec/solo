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
        Ray() = default;
        Ray(const Vector3 &origin, const Vector3 &direction);
        Ray(float originX, float originY, float originZ, float dirX, float dirY, float dirZ);

        auto origin() const -> Vector3 { return origin_; }
        void setOrigin(const Vector3 &origin);

        auto direction() const -> Vector3 { return direction_; }
        void setDirection(const Vector3 &direction);

    private:
        Vector3 origin_{0, 0, 0};
        Vector3 direction_{0, 0, 1};
      
        void normalize();
    };
}
