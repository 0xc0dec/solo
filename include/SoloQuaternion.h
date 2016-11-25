/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

namespace solo
{
    struct Radian;
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
        Quaternion(const Vector3 &axis, const Radian &angle);

        static auto createFromAxisAngle(const Vector3 &axis, const Radian &angle) -> Quaternion;

        static auto identity() -> Quaternion;
        static auto zero() -> Quaternion;

        bool isIdentity() const;
        bool isZero() const;

        void conjugate();
        bool inverse();

        auto normalized() const -> Quaternion;
        void normalize();

        auto toAxisAngle(Vector3 &e) const -> Radian;

        static auto lerp(const Quaternion &q1, const Quaternion &q2, float t) -> Quaternion;
        static auto slerp(const Quaternion &q1, const Quaternion &q2, float t) -> Quaternion;
        static auto squad(const Quaternion &q1, const Quaternion &q2, const Quaternion &s1, const Quaternion &s2, float t) -> Quaternion;

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
