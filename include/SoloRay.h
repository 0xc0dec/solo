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
    class Plane;
    class BoundingSphere;
    class BoundingBox;
    class Frustum;

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

        // These methods return -1 if there's no intersection
        auto hitBoundingSphere(const BoundingSphere &sphere) const -> float;
        auto hitBoundingBox(const BoundingBox &box) const -> float;
        auto hitFrustum(const Frustum &frustum) const -> float;
        auto hitPlane(const Plane &plane) const -> float;

    private:
        void normalize();

        Vector3 origin{0, 0, 0};
        Vector3 direction{0, 0, 1};
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
