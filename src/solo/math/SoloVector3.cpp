/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloVector3.h"

using namespace solo;

Vector3::Vector3(const glm::vec3 &data):
    data_(data)
{
}

Vector3::Vector3(float all):
    data_{all}
{
}

Vector3::Vector3(float x, float y, float z):
    data_{x, y, z}
{
}

auto Vector3::operator+(const Vector3 &v) const -> Vector3
{
    return data_ + v.data_;
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
    data_ = glm::clamp(data_, min.data_, max.data_);
}

auto Vector3::cross(const Vector3 &v) const -> Vector3
{
    return glm::cross(data_, v.data_);
}

auto Vector3::distance(const Vector3 &v) const -> float
{
    return glm::distance(data_, v.data_);
}

auto Vector3::dot(const Vector3 &v) const -> float
{
    return glm::dot(data_, v.data_);
}

auto Vector3::length() const -> float
{
    return glm::length(data_);
}

auto Vector3::normalized() const -> Vector3
{
    return glm::normalize(data_);
}

void Vector3::normalize()
{
    if (!isZero())
        data_ = glm::normalize(data_);
}

auto Vector3::operator=(const glm::vec3 &other) -> Vector3 &
{
    data_ = other;
    return *this;
}

bool Vector3::operator==(const Vector3 &v) const
{
    return glm::all(glm::epsilonEqual(data_, v.data_, glm::epsilon<float>()));
}

bool Vector3::operator!=(const Vector3 &v) const
{
    return !operator==(v);
}

auto Vector3::operator+(float scalar) const -> Vector3
{
    return data_ + scalar;
}

auto Vector3::operator+=(const Vector3 &v) -> Vector3 &
{
    data_ += v.data_;
    return *this;
}

auto Vector3::operator+=(float scalar) -> Vector3 &
{
    data_ += scalar;
    return *this;
}

auto Vector3::operator-() const -> Vector3
{
    return -data_;
}

auto Vector3::operator-(float scalar) const -> Vector3
{
    return data_ - scalar;
}

auto Vector3::operator-(const Vector3 &v) const -> Vector3
{
    return data_ - v.data_;
}

auto Vector3::operator-=(const Vector3 &v) -> Vector3 &
{
    data_ -= v.data_;
    return *this;
}

auto Vector3::operator-=(float scalar) -> Vector3 &
{
    data_ -= scalar;
    return *this;
}

auto Vector3::operator*(float scalar) const -> Vector3
{
    return data_ * scalar;
}

auto Vector3::operator*=(float scalar) -> Vector3 &
{
    data_ *= scalar;
    return *this;
}

auto Vector3::operator/(const float scalar) const -> Vector3
{
    return data_ / scalar;
}

auto Vector3::operator/=(float scalar) -> Vector3 &
{
    data_ /= scalar;
    return *this;
}
