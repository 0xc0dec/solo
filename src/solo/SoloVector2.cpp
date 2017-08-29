/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloMath.h"
#include "SoloVector2.h"

using namespace solo;

Vector2::Vector2(float x, float y):
    x(x), y(y)
{
}

auto Vector2::unit() -> Vector2
{
    static Vector2 value(1.0f, 1.0f);
    return value;
}

auto Vector2::unitX() -> Vector2
{
    static Vector2 value(1.0f, 0.0f);
    return value;
}

auto Vector2::unitY() -> Vector2
{
    static Vector2 value(0.0f, 1.0f);
    return value;
}

bool Vector2::isZero() const
{
    return math::isZero(x, math::epsilon1) && math::isZero(y, math::epsilon1);
}

bool Vector2::isUnit() const
{
    return math::areEqual(x, 1.0f, math::epsilon1) && math::areEqual(y, 1.0f, math::epsilon1);
}

auto Vector2::angle(const Vector2 &v1, const Vector2 &v2) -> Radian
{
    auto dz = v1.x * v2.y - v1.y * v2.x;
    return Radian(atan2f(fabsf(dz) + math::epsilon2, dot(v1, v2)));
}

void Vector2::clamp(const Vector2 &min, const Vector2 &max)
{
    if (x < min.x)
        x = min.x;
    if (x > max.x)
        x = max.x;

    if (y < min.y)
        y = min.y;
    if (y > max.y)
        y = max.y;
}

auto Vector2::distance(const Vector2 &v) const -> float
{
    auto dx = v.x - x;
    auto dy = v.y - y;
    return sqrt(dx * dx + dy * dy);
}

auto Vector2::distanceSquared(const Vector2 &v) const -> float
{
    auto dx = v.x - x;
    auto dy = v.y - y;
    return dx * dx + dy * dy;
}

auto Vector2::dot(const Vector2 &v) const -> float
{
    return x * v.x + y * v.y;
}

auto Vector2::dot(const Vector2 &v1, const Vector2 &v2) -> float
{
    return v1.x * v2.x + v1.y * v2.y;
}

auto Vector2::length() const -> float
{
    return sqrt(x * x + y * y);
}

auto Vector2::lengthSquared() const -> float
{
    return x * x + y * y;
}

auto Vector2::normalized() const -> Vector2
{
    auto result(*this);
    result.normalize();
    return result;
}

void Vector2::normalize()
{
    auto n = x * x + y * y;
    // Already normalized
    if (math::areEqual(n, 1.0f, math::epsilon1))
        return;

    n = sqrt(n);
    if (math::isZero(n, math::epsilon2))
        return;

    n = 1.0f / n;
    x *= n;
    y *= n;
}
