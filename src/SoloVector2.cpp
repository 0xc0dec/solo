#include "SoloMath.h"
#include "SoloVector2.h"

using namespace solo;


Vector2::Vector2(float x, float y):
    x(x), y(y)
{
}


Vector2 Vector2::zero()
{
    static Vector2 value(0.0f, 0.0f);
    return value;
}


Vector2 Vector2::unit()
{
    static Vector2 value(1.0f, 1.0f);
    return value;
}


Vector2 Vector2::unitX()
{
    static Vector2 value(1.0f, 0.0f);
    return value;
}


Vector2 Vector2::unitY()
{
    static Vector2 value(0.0f, 1.0f);
    return value;
}


bool Vector2::isZero() const
{
    return Math::approxZero(x, Math::smallFloat1) && Math::approxZero(y, Math::smallFloat1);
}


bool Vector2::isUnit() const
{
    return Math::approxEqual(x, 1.0f, Math::smallFloat1) && Math::approxEqual(y, 1.0f, Math::smallFloat1);
}


float Vector2::angle(const Vector2& v1, const Vector2& v2)
{
    auto dz = v1.x * v2.y - v1.y * v2.x;
    return atan2f(fabsf(dz) + Math::smallFloat2, dot(v1, v2));
}


void Vector2::clamp(const Vector2& min, const Vector2& max)
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


float Vector2::distance(const Vector2& v) const
{
    auto dx = v.x - x;
    auto dy = v.y - y;
    return sqrt(dx * dx + dy * dy);
}


float Vector2::distanceSquared(const Vector2& v) const
{
    auto dx = v.x - x;
    auto dy = v.y - y;
    return dx * dx + dy * dy;
}


float Vector2::dot(const Vector2& v) const
{
    return x * v.x + y * v.y;
}


float Vector2::dot(const Vector2& v1, const Vector2& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}


float Vector2::length() const
{
    return sqrt(x * x + y * y);
}


float Vector2::lengthSquared() const
{
    return x * x + y * y;
}


Vector2 Vector2::normalized() const
{
    auto result(*this);
    result.normalize();
    return result;
}


void Vector2::normalize()
{
    auto n = x * x + y * y;
    // Already normalized
    if (Math::approxEqual(n, 1.0f, Math::smallFloat1))
        return;

    n = sqrt(n);
    if (Math::approxZero(n, Math::smallFloat2))
        return;

    n = 1.0f / n;
    x *= n;
    y *= n;
}
