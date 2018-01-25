/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVector3.h"

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
    return math::isZero(x) &&
           math::isZero(y) &&
           math::isZero(z);
}

bool Vector3::isUnit() const
{
    return math::areEqual(x, 1.0f) &&
           math::areEqual(y, 1.0f) &&
           math::areEqual(z, 1.0f);
}

auto Vector3::angle(const Vector3 &v) -> Radians
{
    return Radians(acosf(math::clamp(dot(v), -1, 1)));
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
    const auto dx = v.x - x;
    const auto dy = v.y - y;
    const auto dz = v.z - z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

auto Vector3::distanceSquared(const Vector3 &v) const -> float
{
    const auto dx = v.x - x;
    const auto dy = v.y - y;
    const auto dz = v.z - z;
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
    if (math::areEqual(n, 1.0f))
        return;

    n = sqrt(n);
    if (math::isZero(n))
        return;

    n = 1.0f / n;
    x *= n;
    y *= n;
    z *= n;
}
