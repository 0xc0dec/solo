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
#include <vector>


namespace solo
{
    class BoundingSphere;
    class Frustum;
    class Ray;
    class Matrix;

    class BoundingBox final
    {
    public:
        Vector3 min;
        Vector3 max;

        BoundingBox() {}
        BoundingBox(const Vector3 &min, const Vector3 &max);

        static auto empty() -> const BoundingBox&;

        auto getCenter() const -> Vector3;
        auto getCorners() const -> std::vector<Vector3>;

        bool intersectsBoundingBox(const BoundingBox &box) const;
        bool intersectsBoundingSphere(const BoundingSphere &sphere) const;
        bool intersectsFrustum(const Frustum &frustum) const;
        auto hitByRay(const Ray &ray) const -> float;
        auto intersectPlane(const Plane &plane) const -> PlaneIntersection;

        bool isEmpty() const;

        void mergeBoundingSphere(const BoundingSphere &sphere);
        void mergeBoundingBox(const BoundingBox &box);
    };

    inline bool BoundingBox::isEmpty() const
    {
        return math::areEqual(min.x, max.x, math::epsilon1) &&
               math::areEqual(min.y, max.y, math::epsilon1) &&
               math::areEqual(min.z, max.z, math::epsilon1);
    }
}
