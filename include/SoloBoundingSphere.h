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
#include "SoloPlane.h"
#include "SoloMath.h"


namespace solo
{
    class BoundingSphere final
    {
    public:
        Vector3 center;
        float radius = 0;

        static auto empty() -> const BoundingSphere&;

        BoundingSphere() {}
        BoundingSphere(const Vector3 &center, float radius);

        bool intersectsBoundingSphere(const BoundingSphere &sphere) const;
        bool intersectsBoundingBox(const BoundingBox &box) const;
        bool intersectsFrustum(const Frustum &frustum) const;
        auto hitByRay(const Ray &ray) const -> float;
        auto intersectPlane(const Plane &plane) const -> PlaneIntersection;

        bool isEmpty() const;

        void mergeBoundingSphere(const BoundingSphere &sphere);
        void mergeBoundingBox(const BoundingBox &box);
    };

    inline bool BoundingSphere::isEmpty() const
    {
        return Math::approxZero(radius, Math::smallFloat1);
    }
}
