/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVector4.h"

using namespace solo;

Vector4::Vector4(float x, float y, float z, float w):
    x(x), y(y), z(z), w(w)
{
}

bool Vector4::isZero() const
{
    return math::isZero(x) &&
           math::isZero(y) &&
           math::isZero(z) &&
           math::isZero(w);
}

bool Vector4::isUnit() const
{
    return math::areEqual(x, 1.0f) &&
           math::areEqual(y, 1.0f) &&
           math::areEqual(z, 1.0f) &&
           math::areEqual(w, 1.0f);
}

auto Vector4::angle(const Vector4 &v) -> Radian
{
    return Radian(acosf(math::clamp(dot(v), -1, 1)));
}

void Vector4::clamp(const Vector4 &min, const Vector4 &max)
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

    if (w < min.w)
        w = min.w;
    if (w > max.w)
        w = max.w;
}

auto Vector4::distance(const Vector4 &v) const -> float
{
    auto dx = v.x - x;
    auto dy = v.y - y;
    auto dz = v.z - z;
    auto dw = v.w - w;

    return sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

auto Vector4::distanceSquared(const Vector4 &v) const -> float
{
    auto dx = v.x - x;
    auto dy = v.y - y;
    auto dz = v.z - z;
    auto dw = v.w - w;

    return dx * dx + dy * dy + dz * dz + dw * dw;
}

auto Vector4::dot(const Vector4 &v) const -> float
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

auto Vector4::length() const -> float
{
    return sqrt(x * x + y * y + z * z + w * w);
}

auto Vector4::lengthSquared() const -> float
{
    return x * x + y * y + z * z + w * w;
}

auto Vector4::normalized() const -> Vector4
{
    auto result(*this);
    result.normalize();
    return result;
}

void Vector4::normalize()
{
    auto n = x * x + y * y + z * z + w * w;
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
    w *= n;
}
