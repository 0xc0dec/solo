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

#include "SoloFrustum.h"
#include "SoloBoundingSphere.h"
#include "SoloBoundingBox.h"
#include "SoloRay.h"

using namespace solo;


Frustum::Frustum()
{
    setMatrix(Matrix::identity());
}


void Frustum::setMatrix(const Matrix &m)
{
    this->matrix = m;
    updatePlanes();
}


auto Frustum::getCorners() const -> std::vector<Vector3>
{
    auto near = getNearCorners();
    auto far = getFarCorners();
    near.insert(near.end(), far.begin(), far.end());
    return near;
}


auto Frustum::getNearCorners() const -> std::vector<Vector3>
{
    std::vector<Vector3> result;
    result.emplace_back(Plane::getCommonPoint(near, left, top));
    result.emplace_back(Plane::getCommonPoint(near, left, bottom));
    result.emplace_back(Plane::getCommonPoint(near, right, bottom));
    result.emplace_back(Plane::getCommonPoint(near, right, top));
    return result;
}


auto Frustum::getFarCorners() const -> std::vector<Vector3>
{
    std::vector<Vector3> result;
    result.emplace_back(Plane::getCommonPoint(far, right, top));
    result.emplace_back(Plane::getCommonPoint(far, right, bottom));
    result.emplace_back(Plane::getCommonPoint(far, left, bottom));
    result.emplace_back(Plane::getCommonPoint(far, left, top));
    return result;
}


bool Frustum::intersectsPoint(const Vector3 &point) const
{
    if (near.getDistanceToPoint(point) <= 0)
        return false;
    if (far.getDistanceToPoint(point) <= 0)
        return false;
    if (left.getDistanceToPoint(point) <= 0)
        return false;
    if (right.getDistanceToPoint(point) <= 0)
        return false;
    if (top.getDistanceToPoint(point) <= 0)
        return false;
    if (bottom.getDistanceToPoint(point) <= 0)
        return false;

    return true;
}


bool Frustum::intersectsBoundingSphere(const BoundingSphere &sphere) const
{
    return sphere.intersectsFrustum(*this);
}


bool Frustum::intersectsBoundingBox(const BoundingBox &box) const
{
    return box.intersectsFrustum(*this);
}


auto Frustum::intersectPlane(const Plane &plane) const -> PlaneIntersection
{
    return plane.intersectFrustum(*this);
}


auto Frustum::hitByRay(const Ray &ray) const -> float
{
    return ray.hitFrustum(*this);
}


void Frustum::updatePlanes()
{
    near = Plane(Vector3(matrix.m[3] + matrix.m[2], matrix.m[7] + matrix.m[6], matrix.m[11] + matrix.m[10]), matrix.m[15] + matrix.m[14]);
    far = Plane(Vector3(matrix.m[3] - matrix.m[2], matrix.m[7] - matrix.m[6], matrix.m[11] - matrix.m[10]), matrix.m[15] - matrix.m[14]);
    bottom = Plane(Vector3(matrix.m[3] + matrix.m[1], matrix.m[7] + matrix.m[5], matrix.m[11] + matrix.m[9]), matrix.m[15] + matrix.m[13]);
    top = Plane(Vector3(matrix.m[3] - matrix.m[1], matrix.m[7] - matrix.m[5], matrix.m[11] - matrix.m[9]), matrix.m[15] - matrix.m[13]);
    left = Plane(Vector3(matrix.m[3] + matrix.m[0], matrix.m[7] + matrix.m[4], matrix.m[11] + matrix.m[8]), matrix.m[15] + matrix.m[12]);
    right = Plane(Vector3(matrix.m[3] - matrix.m[0], matrix.m[7] - matrix.m[4], matrix.m[11] - matrix.m[8]), matrix.m[15] - matrix.m[12]);
}
