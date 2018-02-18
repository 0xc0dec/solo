/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVector4.h"

using namespace solo;

Vector4::Vector4(const glm::vec4 &data):
	data(data)
{
}

Vector4::Vector4(float all):
	data(all)
{
}

Vector4::Vector4(float x, float y, float z, float w):
    data(x, y, z, w)
{
}

bool Vector4::isZero() const
{
    return math::isZero(x()) &&
           math::isZero(y()) &&
           math::isZero(z()) &&
           math::isZero(w());
}

bool Vector4::isUnit() const
{
    return math::areEqual(x(), 1.0f) &&
           math::areEqual(y(), 1.0f) &&
           math::areEqual(z(), 1.0f) &&
           math::areEqual(w(), 1.0f);
}

auto Vector4::angle(const Vector4 &v) -> Radians
{
    return Radians(std::acosf(glm::clamp(dot(v), -1.0f, 1.0f)));
}

void Vector4::clamp(const Vector4 &min, const Vector4 &max)
{
	data = glm::clamp(data, min.data, max.data);
}

auto Vector4::distance(const Vector4 &v) const -> float
{
	return glm::distance(data, v.data);
}

auto Vector4::dot(const Vector4 &v) const -> float
{
    return glm::dot(data, v.data);
}

auto Vector4::operator=(const glm::vec4 &other) -> Vector4 &
{
	data = other;
	return *this;
}

auto Vector4::length() const -> float
{
    return glm::length(data);
}

auto Vector4::normalized() const -> Vector4
{
	return glm::normalize(data);
}

void Vector4::normalize()
{
	if (!isZero())
		data = glm::normalize(data);
}

bool Vector4::operator==(const Vector4 &v) const
{
    return glm::all(glm::epsilonEqual(data, v.data, glm::epsilon<float>()));
}

bool Vector4::operator!=(const Vector4 &v) const
{
	return !operator==(v);
}

auto Vector4::operator+(float scalar) const -> Vector4
{
	return data + scalar;
}

auto Vector4::operator+(const Vector4 &v) const -> Vector4
{
	return data + v.data;
}

auto Vector4::operator+=(const Vector4 &v) -> Vector4 &
{
	data += v.data;
    return *this;
}

auto Vector4::operator+=(float scalar) -> Vector4 &
{
	data += scalar;
    return *this;
}

auto Vector4::operator-() const -> Vector4
{
	return -data;
}

auto Vector4::operator-(float scalar) const -> Vector4
{
	return data - scalar;
}

auto Vector4::operator-(const Vector4 &v) const -> Vector4
{
	return data - v.data;
}

auto Vector4::operator-=(float scalar) -> Vector4 &
{
	data -= scalar;
    return *this;
}

auto Vector4::operator-=(const Vector4 &v) -> Vector4 &
{
	data -= v.data;
    return *this;
}

auto Vector4::operator*(float scalar) const -> Vector4
{
	return data * scalar;
}

auto Vector4::operator*=(float scalar) -> Vector4 &
{
	data *= scalar;
    return *this;
}

auto Vector4::operator/(const float scalar) const -> Vector4
{
    return data / scalar;
}

auto Vector4::operator/=(const float scalar) -> Vector4 &
{
	data /= scalar;
    return *this;
}