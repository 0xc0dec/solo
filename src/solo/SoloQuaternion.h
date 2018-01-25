/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

namespace solo
{
    struct Radians;
    class Vector3;

    class Quaternion final
    {
    public:
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 1;

        Quaternion() {}
        Quaternion(float x, float y, float z, float w);
        Quaternion(const Vector3 &axis, const Radians &angle);

        static auto createFromAxisAngle(const Vector3 &axis, const Radians &angle) -> Quaternion;
        static auto lerp(const Quaternion &q1, const Quaternion &q2, float t) -> Quaternion;
        static auto slerp(const Quaternion &q1, const Quaternion &q2, float t) -> Quaternion;
        static auto squad(const Quaternion &q1, const Quaternion &q2, const Quaternion &s1, const Quaternion &s2, float t) -> Quaternion;

        bool isIdentity() const;
        bool isZero() const;

        void conjugate();
        bool inverse();

        auto normalized() const -> Quaternion;
        void normalize();

        auto toAxisAngle(Vector3 &e) const -> Radians;

        auto operator*(const Quaternion &q) const -> Quaternion;
        auto operator*=(const Quaternion &q) -> Quaternion&;

    private:
        static auto slerpForSquad(const Quaternion &q1, const Quaternion &q2, float t) -> Quaternion;
    };

    inline auto Quaternion::operator*(const Quaternion &q) const -> Quaternion
    {
        auto result(*this);
        result *= q;
        return result;
    }
}
