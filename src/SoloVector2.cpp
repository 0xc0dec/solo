#include "SoloMath.h"
#include "SoloVector2.h"

using namespace solo;


Vector2::Vector2(float x, float y):
	x{x}, y{y}
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
	return x == 0.0f && y == 0.0f;
}


bool Vector2::isUnit() const
{
	return x == 1.0f && y == 1.0f;
}


float Vector2::angle(const Vector2& v1, const Vector2& v2)
{
	auto dz = v1.x * v2.y - v1.y * v2.x;
	return atan2f(fabsf(dz) + Math::FLOAT_SMALL, dot(v1, v2));
}


void Vector2::clamp(const Vector2& min, const Vector2& max)
{
	// Clamp the x value.
	if (x < min.x)
		x = min.x;
	if (x > max.x)
		x = max.x;

	// Clamp the y value.
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
	return (dx * dx + dy * dy);
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
	auto result = *this;
	result.normalize();
	return result;
}


void Vector2::normalize()
{
	auto n = x * x + y * y;
	// Already normalized.
	if (n == 1.0f)
		return;

	n = sqrt(n);
	// Too close to zero.
	if (n < Math::TOLERANCE)
		return;

	n = 1.0f / n;
	x *= n;
	y *= n;
}
