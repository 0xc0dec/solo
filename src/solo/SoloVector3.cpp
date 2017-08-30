/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVector3.h"
#include "SoloMath.h"

using namespace solo;

Vector3::Vector3(float all):
    x(all), y(all), z(all)
{
}

Vector3::Vector3(float x, float y, float z):
    x(x), y(y), z(z)
{
}

bool Vector3::isZero() const
{
    return math::isZero(x, math::epsilon1) &&
           math::isZero(y, math::epsilon1) &&
           math::isZero(z, math::epsilon1);
}

bool Vector3::isUnit() const
{
    return math::areEqual(x, 1.0f, math::epsilon1) &&
           math::areEqual(y, 1.0f, math::epsilon1) &&
           math::areEqual(z, 1.0f, math::epsilon1);
}

auto Vector3::angle(const Vector3 &v) -> Radian
{
    auto dx = y * v.z - z * v.y;
    auto dy = z * v.x - x * v.z;
    auto dz = x * v.y - y * v.x;

    return Radian(atan2f(sqrt(dx * dx + dy * dy + dz * dz) + math::epsilon2, dot(v)));
}

void Vector3::clamp(const Vector3 &min, const Vector3 &max)
{
    if (x < min.x)
        x = min.x;
    if (x > max.x)
        x = max.x;

    if (y < min.y)
        y = min.y;
    if (y > max.y)
        y = max.y;

    if (z < min.z)
        z = min.z;
    if (z > max.z)
        z = max.z;
}

auto Vector3::cross(const Vector3 &v) -> Vector3
{
    return Vector3(
        (y * v.z) - (z * v.y),
        (z * v.x) - (x * v.z),
        (x * v.y) - (y * v.x)
    );
}

auto Vector3::distance(const Vector3 &v) const -> float
{
    auto dx = v.x - x;
    auto dy = v.y - y;
    auto dz = v.z - z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

auto Vector3::distanceSquared(const Vector3 &v) const -> float
{
    auto dx = v.x - x;
    auto dy = v.y - y;
    auto dz = v.z - z;
    return dx * dx + dy * dy + dz * dz;
}

auto Vector3::dot(const Vector3 &v) const -> float
{
    return x * v.x + y * v.y + z * v.z;
}

auto Vector3::length() const -> float
{
    return sqrt(x * x + y * y + z * z);
}

auto Vector3::lengthSquared() const -> float
{
    return x * x + y * y + z * z;
}

auto Vector3::normalized() const -> Vector3
{
    auto result(*this);
    result.normalize();
    return result;
}

void Vector3::normalize()
{
    auto n = x * x + y * y + z * z;
    // Already normalized
    if (math::areEqual(n, 1.0f, math::epsilon1))
        return;

    n = sqrt(n);
    if (math::isZero(n, math::epsilon3))
        return;

    n = 1.0f / n;
    x *= n;
    y *= n;
    z *= n;
}
