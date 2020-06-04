/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVector2.h"

using namespace solo;

Vector2::Vector2(float x, float y):
    data_(x, y)
{
}

Vector2::Vector2(const glm::vec2 &data):
    data_(data)
{
}

Vector2::Vector2(float all):
    data_(all)
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
    data_ = glm::clamp(data_, min.data_, max.data_);
}

auto Vector2::distance(const Vector2 &v) const -> float
{
    return glm::distance(data_, v.data_);
}

auto Vector2::dot(const Vector2 &v) const -> float
{
    return glm::dot(data_, v.data_);
}

auto Vector2::operator=(const glm::vec2 &other) -> Vector2 &
{
    data_ = other;
    return *this;
}

auto Vector2::length() const -> float
{
    return glm::length(data_);
}

auto Vector2::normalized() const -> Vector2
{
    return glm::normalize(data_);
}

void Vector2::normalize()
{
    if (!isZero())
        data_ = glm::normalize(data_);
}

bool Vector2::operator==(const Vector2 &v) const
{
    return glm::all(glm::epsilonEqual(data_, v.data_, glm::epsilon<float>()));
}

bool Vector2::operator!=(const Vector2 &v) const
{
    return !operator==(v);
}

auto Vector2::operator+(const Vector2 &v) const -> Vector2
{
    return data_ + v.data_;
}

auto Vector2::operator+=(float scalar) -> Vector2 &
{
    data_ += scalar;
    return *this;
}

auto Vector2::operator+=(const Vector2 &v) -> Vector2 &
{
    data_ += v.data_;
    return *this;
}

auto Vector2::operator-() const -> Vector2
{
    return -data_;
}

auto Vector2::operator-(const Vector2 &v) const -> Vector2
{
    return data_ - v.data_;
}

auto Vector2::operator-=(float scalar) -> Vector2 &
{
    data_ -= scalar;
    return *this;
}

auto Vector2::operator-=(const Vector2 &v) -> Vector2 &
{
    data_ -= v.data_;
    return *this;
}

auto Vector2::operator*(float scalar) const -> Vector2
{
    return data_ * scalar;
}

auto Vector2::operator*=(float scalar) -> Vector2 &
{
    data_ *= scalar;
    return *this;
}

auto Vector2::operator/(const float scalar) const -> Vector2
{
    return data_ / scalar;
}

auto Vector2::operator/=(float scalar) -> Vector2 &
{
    data_ /= scalar;
    return *this;
}
