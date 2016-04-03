#pragma once

#include "SoloMatrix.h"


namespace solo
{
    class Quaternion final
    {
    public:
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 1;

        Quaternion() {}
        Quaternion(float x, float y, float z, float w);
        Quaternion(const Vector3& axis, float angleRadians);

        static auto createFromRotationMatrix(const Matrix& m) -> Quaternion;
        static auto createFromAxisAngle(const Vector3& axis, float angleRadians) -> Quaternion;

        static auto identity() -> Quaternion;
        static auto zero() -> Quaternion;

        bool isIdentity() const;
        bool isZero() const;

        void conjugate();
        bool inverse();

        auto normalized() const -> Quaternion;
        void normalize();

        auto toAxisAngle(Vector3& e) const -> float;

        static auto lerp(const Quaternion& q1, const Quaternion& q2, float t) -> Quaternion;
        static auto slerp(const Quaternion& q1, const Quaternion& q2, float t) -> Quaternion;
        static auto squad(const Quaternion& q1, const Quaternion& q2, const Quaternion& s1, const Quaternion& s2, float t) -> Quaternion;

        auto operator*(const Quaternion& q) const -> Quaternion;
        auto operator*=(const Quaternion& q) -> Quaternion&;

    private:
        static auto slerpForSquad(const Quaternion& q1, const Quaternion& q2, float t) -> Quaternion;
    };

    inline auto Quaternion::operator*(const Quaternion& q) const -> Quaternion
    {
        auto result(*this);
        result *= q;
        return result;
    }
}
