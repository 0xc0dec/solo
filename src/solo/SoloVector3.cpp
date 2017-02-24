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


auto Vector3::unit() -> Vector3
{
    static Vector3 value(1.0f, 1.0f, 1.0f);
    return value;
}


auto Vector3::unitX() -> Vector3
{
    static Vector3 value(1.0f, 0.0f, 0.0f);
    return value;
}


auto Vector3::unitY() -> Vector3
{
    static Vector3 value(0.0f, 1.0f, 0.0f);
    return value;
}


auto Vector3::unitZ() -> Vector3
{
    static Vector3 value(0.0f, 0.0f, 1.0f);
    return value;
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


auto Vector3::angle(const Vector3 &v1, const Vector3 &v2) -> Radian
{
    auto dx = v1.y * v2.z - v1.z * v2.y;
    auto dy = v1.z * v2.x - v1.x * v2.z;
    auto dz = v1.x * v2.y - v1.y * v2.x;

    return Radian(atan2f(sqrt(dx * dx + dy * dy + dz * dz) + math::epsilon2, dot(v1, v2)));
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


auto Vector3::cross(const Vector3 &v1, const Vector3 &v2) -> Vector3
{
    return Vector3(
        (v1.y * v2.z) - (v1.z * v2.y),
        (v1.z * v2.x) - (v1.x * v2.z),
        (v1.x * v2.y) - (v1.y * v2.x)
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


auto Vector3::dot(const Vector3 &v1, const Vector3 &v2) -> float
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
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
