/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloQuaternion.h"
#include "SoloMath.h"
#include "SoloRadians.h"
#include "SoloVector3.h"

using namespace solo;

Quaternion::Quaternion(float x, float y, float z, float w):
    data_{x, y, z, w}
{
}

Quaternion::Quaternion(const glm::quat &data):
    data_(data)
{
}

Quaternion::Quaternion(const Vector3 &axis, const Radians &angle)
{
    *this = fromAxisAngle(axis, angle);
}

bool Quaternion::isIdentity() const
{
    return math::isZero(x()) &&
           math::isZero(y()) &&
           math::isZero(z()) &&
           math::areEqual(w(), 1.0f);
}

bool Quaternion::isZero() const
{
    return math::isZero(x()) &&
           math::isZero(y()) &&
           math::isZero(z()) &&
           math::isZero(w());
}

auto Quaternion::fromAxisAngle(const Vector3 &axis, const Radians &angle) -> Quaternion
{
    return glm::angleAxis(angle.toRawRadians(), static_cast<glm::vec3>(axis));
}

void Quaternion::conjugate()
{
    x() = -x();
    y() = -y();
    z() = -z();
}

void Quaternion::invert()
{
    data_ = glm::inverse(data_);
}

void Quaternion::normalize()
{
    data_ = glm::normalize(data_);
}

auto Quaternion::normalized() const -> Quaternion
{
    return glm::normalize(data_);
}

auto Quaternion::toAxisAngle(Vector3 &axis) const -> Radians
{
    const auto q = normalized();
    axis.x() = q.x();
    axis.y() = q.y();
    axis.z() = q.z();
    axis.normalize();
    return Radians(2.0f * std::acos(q.w()));
}

auto Quaternion::lerp(const Quaternion &q1, const Quaternion &q2, float t) -> Quaternion
{
    return glm::lerp(static_cast<glm::quat>(q1), static_cast<glm::quat>(q2), t);
}

auto Quaternion::slerp(const Quaternion &q1, const Quaternion &q2, float t) -> Quaternion
{
    return glm::slerp(static_cast<glm::quat>(q1), static_cast<glm::quat>(q2), t);
}

auto Quaternion::operator*=(const Quaternion &q) -> Quaternion &
{
    data_ *= q.data_;
    return *this;
}

auto Quaternion::operator*(const Quaternion &q) const -> Quaternion
{
    return data_ * q.data_;
}