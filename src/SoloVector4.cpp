#include "SoloVector4.h"
#include "SoloMath.h"

using namespace solo;


Vector4::Vector4(float x, float y, float z, float w):
	x(x), y(y), z(z), w(w)
{
}


Vector4::Vector4(const float* src)
{
	set(src);
}


Vector4::Vector4(const Vector4& p1, const Vector4& p2)
{
	*this = p2 - p1;
}


const Vector4& Vector4::zero()
{
	static Vector4 value(0.0f, 0.0f, 0.0f, 0.0f);
	return value;
}


const Vector4& Vector4::unit()
{
	static Vector4 value(1.0f, 1.0f, 1.0f, 1.0f);
	return value;
}


const Vector4& Vector4::unitX()
{
	static Vector4 value(1.0f, 0.0f, 0.0f, 0.0f);
	return value;
}


const Vector4& Vector4::unitY()
{
	static Vector4 value(0.0f, 1.0f, 0.0f, 0.0f);
	return value;
}


const Vector4& Vector4::unitZ()
{
	static Vector4 value(0.0f, 0.0f, 1.0f, 0.0f);
	return value;
}


const Vector4& Vector4::unitW()
{
	static Vector4 value(0.0f, 0.0f, 0.0f, 1.0f);
	return value;
}


bool Vector4::isZero() const
{
	return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
}


bool Vector4::isUnit() const
{
	return x == 1.0f && y == 1.0f && z == 1.0f && w == 1.0f;
}


float Vector4::angle(const Vector4& v1, const Vector4& v2)
{
	auto dx = v1.w * v2.x - v1.x * v2.w - v1.y * v2.z + v1.z * v2.y;
	auto dy = v1.w * v2.y - v1.y * v2.w - v1.z * v2.x + v1.x * v2.z;
	auto dz = v1.w * v2.z - v1.z * v2.w - v1.x * v2.y + v1.y * v2.x;

	return atan2f(sqrt(dx * dx + dy * dy + dz * dz) + MATH_FLOAT_SMALL, dot(v1, v2));
}


void Vector4::clamp(const Vector4& min, const Vector4& max)
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

	// Clamp the z value.
	if (z < min.z)
		z = min.z;
	if (z > max.z)
		z = max.z;

	// Clamp the z value.
	if (w < min.w)
		w = min.w;
	if (w > max.w)
		w = max.w;
}


float Vector4::distance(const Vector4& v) const
{
	auto dx = v.x - x;
	auto dy = v.y - y;
	auto dz = v.z - z;
	auto dw = v.w - w;

	return sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}


float Vector4::distanceSquared(const Vector4& v) const
{
	auto dx = v.x - x;
	auto dy = v.y - y;
	auto dz = v.z - z;
	auto dw = v.w - w;

	return (dx * dx + dy * dy + dz * dz + dw * dw);
}


float Vector4::dot(const Vector4& v) const
{
	return (x * v.x + y * v.y + z * v.z + w * v.w);
}


float Vector4::dot(const Vector4& v1, const Vector4& v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
}


float Vector4::length() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}


float Vector4::lengthSquared() const
{
	return (x * x + y * y + z * z + w * w);
}


Vector4 Vector4::normalized() const
{
	Vector4 result;
	result.normalize();
	return result;
}


void Vector4::normalize()
{
	auto n = x * x + y * y + z * z + w * w;
	// Already normalized.
	if (n == 1.0f)
		return;

	n = sqrt(n);
	// Too close to zero.
	if (n < MATH_TOLERANCE)
		return;

	n = 1.0f / n;
	x *= n;
	y *= n;
	z *= n;
	w *= n;
}


void Vector4::set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


void Vector4::set(const float* array)
{
	x = array[0];
	y = array[1];
	z = array[2];
	w = array[3];
}


void Vector4::set(const Vector4& v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;
}
