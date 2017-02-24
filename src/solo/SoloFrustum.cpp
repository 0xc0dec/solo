/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
    near = {{matrix.m[3] + matrix.m[2], matrix.m[7] + matrix.m[6], matrix.m[11] + matrix.m[10]}, matrix.m[15] + matrix.m[14]};
    far = {{matrix.m[3] - matrix.m[2], matrix.m[7] - matrix.m[6], matrix.m[11] - matrix.m[10]}, matrix.m[15] - matrix.m[14]};
    bottom = {{matrix.m[3] + matrix.m[1], matrix.m[7] + matrix.m[5], matrix.m[11] + matrix.m[9]}, matrix.m[15] + matrix.m[13]};
    top = {{matrix.m[3] - matrix.m[1], matrix.m[7] - matrix.m[5], matrix.m[11] - matrix.m[9]}, matrix.m[15] - matrix.m[13]};
    left = {{matrix.m[3] + matrix.m[0], matrix.m[7] + matrix.m[4], matrix.m[11] + matrix.m[8]}, matrix.m[15] + matrix.m[12]};
    right = {{matrix.m[3] - matrix.m[0], matrix.m[7] - matrix.m[4], matrix.m[11] - matrix.m[8]}, matrix.m[15] - matrix.m[12]};
}
