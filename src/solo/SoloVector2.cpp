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

#include "SoloMath.h"
#include "SoloVector2.h"

using namespace solo;


Vector2::Vector2(float x, float y):
    x(x), y(y)
{
}


auto Vector2::zero() -> Vector2
{
    static Vector2 value(0.0f, 0.0f);
    return value;
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
    return Math::approxZero(x, Math::epsilon1) && Math::approxZero(y, Math::epsilon1);
}


bool Vector2::isUnit() const
{
    return Math::approxEqual(x, 1.0f, Math::epsilon1) && Math::approxEqual(y, 1.0f, Math::epsilon1);
}


auto Vector2::angle(const Vector2 &v1, const Vector2 &v2) -> Radian
{
    auto dz = v1.x * v2.y - v1.y * v2.x;
    return Radian(atan2f(fabsf(dz) + Math::epsilon2, dot(v1, v2)));
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
    if (Math::approxEqual(n, 1.0f, Math::epsilon1))
        return;

    n = sqrt(n);
    if (Math::approxZero(n, Math::epsilon2))
        return;

    n = 1.0f / n;
    x *= n;
    y *= n;
}
