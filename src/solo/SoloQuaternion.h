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
        Quaternion(float x, float y, float z, float w);
        Quaternion(const Vector3 &axis, const Radians &angle);

        static auto createFromAxisAngle(const Vector3 &axis, const Radians &angle) -> Quaternion;
        static auto lerp(const Quaternion &q1, const Quaternion &q2, float t) -> Quaternion;
        static auto slerp(const Quaternion &q1, const Quaternion &q2, float t) -> Quaternion;

		auto x() -> float& { return data.x; }
		auto x() const -> float { return data.x; }
		auto y() -> float& { return data.y; }
		auto y() const -> float { return data.y; }
		auto z() -> float& { return data.z; }
		auto z() const -> float { return data.z; }
		auto w() -> float& { return data.w; }
		auto w() const -> float { return data.w; }

        bool isIdentity() const;
        bool isZero() const;

        void conjugate();
        void invert();

        auto normalized() const -> Quaternion;
        void normalize();

        auto toAxisAngle(Vector3 &e) const -> Radians;

		operator glm::quat() const { return data; }

        auto operator*(const Quaternion &q) const -> Quaternion;
        auto operator*=(const Quaternion &q) -> Quaternion&;

    private:
		glm::quat data;

		Quaternion(const glm::quat &data);
    };
}
