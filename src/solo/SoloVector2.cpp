/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVector2.h"

using namespace solo;

Vector2::Vector2(float x, float y):
    data(x, y)
{
}

Vector2::Vector2(const glm::vec2 &data):
    data(data)
{
}

Vector2::Vector2(float all):
    data(all)
{
}

bool Vector2::isZero() const
{
    return math::isZero(x()) && math::isZero(y());
}

bool Vector2::isUnit() const
{
    return math::areEqual(x(), 1.0f) && math::areEqual(y(), 1.0f);
}

auto Vector2::angle(const Vector2 &v) const -> Radians
{
    return Radians(std::acosf(glm::clamp(dot(v), -1.0f, 1.0f)));
}

void Vector2::clamp(const Vector2 &min, const Vector2 &max)
{
    data = glm::clamp(data, min.data, max.data);
}

auto Vector2::distance(const Vector2 &v) const -> float
{
    return glm::distance(data, v.data);
}

auto Vector2::dot(const Vector2 &v) const -> float
{
    return glm::dot(data, v.data);
}

auto Vector2::operator=(const glm::vec2 &other) -> Vector2 &
{
    data = other;
    return *this;
}

auto Vector2::length() const -> float
{
    return glm::length(data);
}

auto Vector2::normalized() const -> Vector2
{
    return glm::normalize(data);
}

void Vector2::normalize()
{
    if (!isZero())
        data = glm::normalize(data);
}

bool Vector2::operator==(const Vector2 &v) const
{
    return glm::all(glm::epsilonEqual(data, v.data, glm::epsilon<float>()));
}

bool Vector2::operator!=(const Vector2 &v) const
{
    return !operator==(v);
}

auto Vector2::operator+(const Vector2 &v) const -> Vector2
{
    return data + v.data;
}

auto Vector2::operator+=(float scalar) -> Vector2 &
{
    data += scalar;
    return *this;
}

auto Vector2::operator+=(const Vector2 &v) -> Vector2 &
{
    data += v.data;
    return *this;
}

auto Vector2::operator-() const -> Vector2
{
    return -data;
}

auto Vector2::operator-(const Vector2 &v) const -> Vector2
{
    return data - v.data;
}

auto Vector2::operator-=(float scalar) -> Vector2 &
{
    data -= scalar;
    return *this;
}

auto Vector2::operator-=(const Vector2 &v) -> Vector2 &
{
    data -= v.data;
    return *this;
}

auto Vector2::operator*(float scalar) const -> Vector2
{
    return data * scalar;
}

auto Vector2::operator*=(float scalar) -> Vector2 &
{
    data *= scalar;
    return *this;
}

auto Vector2::operator/(const float scalar) const -> Vector2
{
    return data / scalar;
}

auto Vector2::operator/=(float scalar) -> Vector2 &
{
    data /= scalar;
    return *this;
}
