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


Vector3 Vector3::zero()
{
    static Vector3 value(0.0f, 0.0f, 0.0f);
    return value;
}


Vector3 Vector3::unit()
{
    static Vector3 value(1.0f, 1.0f, 1.0f);
    return value;
}


Vector3 Vector3::unitX()
{
    static Vector3 value(1.0f, 0.0f, 0.0f);
    return value;
}


Vector3 Vector3::unitY()
{
    static Vector3 value(0.0f, 1.0f, 0.0f);
    return value;
}


Vector3 Vector3::unitZ()
{
    static Vector3 value(0.0f, 0.0f, 1.0f);
    return value;
}


bool Vector3::isZero() const
{
    return Math::approxZero(x) && Math::approxZero(y) && Math::approxZero(z);
}


bool Vector3::isUnit() const
{
    return Math::approxEqual(x, 1.0f) && Math::approxEqual(y, 1.0f) && Math::approxEqual(z, 1.0f);
}


float Vector3::angle(const Vector3& v1, const Vector3& v2)
{
    auto dx = v1.y * v2.z - v1.z * v2.y;
    auto dy = v1.z * v2.x - v1.x * v2.z;
    auto dz = v1.x * v2.y - v1.y * v2.x;

    return atan2f(sqrt(dx * dx + dy * dy + dz * dz) + Math::SMALL_FLOAT2, dot(v1, v2));
}


void Vector3::clamp(const Vector3& min, const Vector3& max)
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


Vector3 Vector3::cross(const Vector3& v1, const Vector3& v2)
{
    return Vector3(
               (v1.y * v2.z) - (v1.z * v2.y),
               (v1.z * v2.x) - (v1.x * v2.z),
               (v1.x * v2.y) - (v1.y * v2.x)
           );
}


float Vector3::distance(const Vector3& v) const
{
    auto dx = v.x - x;
    auto dy = v.y - y;
    auto dz = v.z - z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}


float Vector3::distanceSquared(const Vector3& v) const
{
    auto dx = v.x - x;
    auto dy = v.y - y;
    auto dz = v.z - z;
    return dx * dx + dy * dy + dz * dz;
}


float Vector3::dot(const Vector3& v) const
{
    return x * v.x + y * v.y + z * v.z;
}


float Vector3::dot(const Vector3& v1, const Vector3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


float Vector3::length() const
{
    return sqrt(x * x + y * y + z * z);
}


float Vector3::lengthSquared() const
{
    return x * x + y * y + z * z;
}


Vector3 Vector3::normalized() const
{
    auto result = *this;
    result.normalize();
    return result;
}


void Vector3::normalize()
{
    auto n = x * x + y * y + z * z;
    // Already normalized
    if (Math::approxEqual(n, 1.0f))
        return;

    n = sqrt(n);
    if (Math::approxZero(n, Math::SMALL_FLOAT2))
        return;

    n = 1.0f / n;
    x *= n;
    y *= n;
    z *= n;
}
