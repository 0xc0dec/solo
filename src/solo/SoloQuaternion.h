/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include <glm/gtc/quaternion.hpp>

namespace solo
{
    struct Radians;
    class Vector3;

    class Quaternion final
    {
    public:
        Quaternion() = default;
        Quaternion(const glm::quat &data);
        Quaternion(float x, float y, float z, float w);
        Quaternion(const Vector3 &axis, const Radians &angle);

        static auto createFromAxisAngle(const Vector3 &axis, const Radians &angle) -> Quaternion;
        static auto lerp(const Quaternion &q1, const Quaternion &q2, float t) -> Quaternion;
        static auto slerp(const Quaternion &q1, const Quaternion &q2, float t) -> Quaternion;

        auto x() -> float& { return data_.x; }
        auto x() const -> float { return data_.x; }
        auto y() -> float& { return data_.y; }
        auto y() const -> float { return data_.y; }
        auto z() -> float& { return data_.z; }
        auto z() const -> float { return data_.z; }
        auto w() -> float& { return data_.w; }
        auto w() const -> float { return data_.w; }

        bool isIdentity() const;
        bool isZero() const;

        void conjugate();
        void invert();

        auto normalized() const -> Quaternion;
        void normalize();

        auto toAxisAngle(Vector3 &e) const -> Radians;

        operator glm::quat() const { return data_; }

        auto operator*(const Quaternion &q) const -> Quaternion;
        auto operator*=(const Quaternion &q) -> Quaternion&;

    private:
        glm::quat data_;
    };
}
