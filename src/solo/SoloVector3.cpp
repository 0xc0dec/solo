/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVector3.h"

using namespace solo;

Vector3::Vector3(const glm::vec3 &data):
    data(data)
{
}

Vector3::Vector3(float all):
    data{all}
{
}

Vector3::Vector3(float x, float y, float z):
    data{x, y, z}
{
}

auto Vector3::operator+(const Vector3 &v) const -> Vector3
{
    return data + v.data;
}

bool Vector3::isZero() const
{
    return math::isZero(x()) &&
           math::isZero(y()) &&
           math::isZero(z());
}

bool Vector3::isUnit() const
{
    return math::areEqual(x(), 1.0f) &&
           math::areEqual(y(), 1.0f) &&
           math::areEqual(z(), 1.0f);
}

auto Vector3::angle(const Vector3 &v) const -> Radians
{
    return Radians(std::acosf(glm::clamp(dot(v), -1.0f, 1.0f)));
}

void Vector3::clamp(const Vector3 &min, const Vector3 &max)
{
    data = glm::clamp(data, min.data, max.data);
}

auto Vector3::cross(const Vector3 &v) const -> Vector3
{
    return glm::cross(data, v.data);
}

auto Vector3::distance(const Vector3 &v) const -> float
{
    return glm::distance(data, v.data);
}

auto Vector3::dot(const Vector3 &v) const -> float
{
    return glm::dot(data, v.data);
}

auto Vector3::length() const -> float
{
    return glm::length(data);
}

auto Vector3::normalized() const -> Vector3
{
    return glm::normalize(data);
}

void Vector3::normalize()
{
    if (!isZero())
        data = glm::normalize(data);
}

auto Vector3::operator=(const glm::vec3 &other) -> Vector3 &
{
    data = other;
    return *this;
}

bool Vector3::operator==(const Vector3 &v) const
{
    return glm::all(glm::epsilonEqual(data, v.data, glm::epsilon<float>()));
}

bool Vector3::operator!=(const Vector3 &v) const
{
    return !operator==(v);
}

auto Vector3::operator+(float scalar) const -> Vector3
{
    return data + scalar;
}

auto Vector3::operator+=(const Vector3 &v) -> Vector3 &
{
    data += v.data;
    return *this;
}

auto Vector3::operator+=(float scalar) -> Vector3 &
{
    data += scalar;
    return *this;
}

auto Vector3::operator-() const -> Vector3
{
    return -data;
}

auto Vector3::operator-(float scalar) const -> Vector3
{
    return data - scalar;
}

auto Vector3::operator-(const Vector3 &v) const -> Vector3
{
    return data - v.data;
}

auto Vector3::operator-=(const Vector3 &v) -> Vector3 &
{
    data -= v.data;
    return *this;
}

auto Vector3::operator-=(float scalar) -> Vector3 &
{
    data -= scalar;
    return *this;
}

auto Vector3::operator*(float scalar) const -> Vector3
{
    return data * scalar;
}

auto Vector3::operator*=(float scalar) -> Vector3 &
{
    data *= scalar;
    return *this;
}

auto Vector3::operator/(const float scalar) const -> Vector3
{
    return data / scalar;
}

auto Vector3::operator/=(float scalar) -> Vector3 &
{
    data /= scalar;
    return *this;
}
