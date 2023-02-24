/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloVector4.h"

using namespace solo;

Vector4::Vector4(const glm::vec4 &data):
    data_(data)
{
}

Vector4::Vector4(float all):
    data_(all)
{
}

Vector4::Vector4(float x, float y, float z, float w):
    data_(x, y, z, w)
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
    data_ = glm::clamp(data_, min.data_, max.data_);
}

auto Vector4::distance(const Vector4 &v) const -> float
{
    return glm::distance(data_, v.data_);
}

auto Vector4::dot(const Vector4 &v) const -> float
{
    return glm::dot(data_, v.data_);
}

auto Vector4::operator=(const glm::vec4 &other) -> Vector4 &
{
    data_ = other;
    return *this;
}

auto Vector4::length() const -> float
{
    return glm::length(data_);
}

auto Vector4::normalized() const -> Vector4
{
    return glm::normalize(data_);
}

void Vector4::normalize()
{
    if (!isZero())
        data_ = glm::normalize(data_);
}

bool Vector4::operator==(const Vector4 &v) const
{
    return glm::all(glm::epsilonEqual(data_, v.data_, glm::epsilon<float>()));
}

bool Vector4::operator!=(const Vector4 &v) const
{
    return !operator==(v);
}

auto Vector4::operator+(float scalar) const -> Vector4
{
    return data_ + scalar;
}

auto Vector4::operator+(const Vector4 &v) const -> Vector4
{
    return data_ + v.data_;
}

auto Vector4::operator+=(const Vector4 &v) -> Vector4 &
{
    data_ += v.data_;
    return *this;
}

auto Vector4::operator+=(float scalar) -> Vector4 &
{
    data_ += scalar;
    return *this;
}

auto Vector4::operator-() const -> Vector4
{
    return -data_;
}

auto Vector4::operator-(float scalar) const -> Vector4
{
    return data_ - scalar;
}

auto Vector4::operator-(const Vector4 &v) const -> Vector4
{
    return data_ - v.data_;
}

auto Vector4::operator-=(float scalar) -> Vector4 &
{
    data_ -= scalar;
    return *this;
}

auto Vector4::operator-=(const Vector4 &v) -> Vector4 &
{
    data_ -= v.data_;
    return *this;
}

auto Vector4::operator*(float scalar) const -> Vector4
{
    return data_ * scalar;
}

auto Vector4::operator*=(float scalar) -> Vector4 &
{
    data_ *= scalar;
    return *this;
}

auto Vector4::operator/(const float scalar) const -> Vector4
{
    return data_ / scalar;
}

auto Vector4::operator/=(const float scalar) -> Vector4 &
{
    data_ /= scalar;
    return *this;
}