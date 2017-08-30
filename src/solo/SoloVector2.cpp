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

bool Vector2::isZero() const
{
    return math::isZero(x) && math::isZero(y);
}

bool Vector2::isUnit() const
{
    return math::areEqual(x, 1.0f) && math::areEqual(y, 1.0f);
}

auto Vector2::angle(const Vector2 &v) const -> Radian
{
    return Radian(acosf(math::clamp(dot(v), -1, 1)));
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
    if (math::areEqual(n, 1.0f))
        return;

    n = sqrt(n);
    if (math::isZero(n))
        return;

    n = 1.0f / n;
    x *= n;
    y *= n;
}
