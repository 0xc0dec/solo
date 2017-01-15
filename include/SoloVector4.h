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

#include "SoloRadian.h"


namespace solo
{
    class Vector4 final
    {
    public:
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;

        Vector4() {}
        Vector4(float x, float y, float z, float w);

        static auto zero() -> Vector4;
        static auto unit() -> Vector4;
        static auto unitX() -> Vector4;
        static auto unitY() -> Vector4;
        static auto unitZ() -> Vector4;
        static auto unitW() -> Vector4;

        bool isZero() const;
        bool isUnit() const;

        static auto angle(const Vector4 &v1, const Vector4 &v2) -> Radian;

        void clamp(const Vector4 &min, const Vector4 &max);

        auto distance(const Vector4 &v) const -> float;
        auto distanceSquared(const Vector4 &v) const -> float;

        auto dot(const Vector4 &v) const -> float;
        static auto dot(const Vector4 &v1, const Vector4 &v2) -> float;

        auto length() const -> float;
        auto lengthSquared() const -> float;

        auto normalized() const -> Vector4;
        void normalize();

        auto operator+(float scalar) const -> Vector4;
        auto operator+(const Vector4 &v) const -> Vector4;
        auto operator+=(float scalar) -> Vector4&;
        auto operator+=(const Vector4 &v) -> Vector4&;

        auto operator-() const -> Vector4;
        auto operator-(float scalar) const -> Vector4;
        auto operator-(const Vector4 &v) const -> Vector4;
        auto operator-=(float scalar)  -> Vector4&;
        auto operator-=(const Vector4 &v) -> Vector4&;

        auto operator*(float x) const -> Vector4;
        auto operator*=(float x) -> Vector4&;

        auto operator/(float x) const -> Vector4;
        auto operator/=(float x) -> Vector4&;

        bool operator<(const Vector4 &v) const;

        bool operator==(const Vector4 &v) const;
        bool operator!=(const Vector4 &v) const;
    };

    inline auto Vector4::operator+(float scalar) const -> Vector4
    {
        auto result(*this);
        result += scalar;
        return result;
    }

    inline auto Vector4::operator+(const Vector4 &v) const -> Vector4
    {
        auto result(*this);
        result += v;
        return result;
    }

    inline auto operator+(float scalar, const Vector4 &v) -> Vector4
    {
        return {v.x + scalar, v.y + scalar, v.z + scalar, v.w + scalar};
    }

    inline auto Vector4::operator+=(const Vector4 &v) -> Vector4 &
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }

    inline auto Vector4::operator+=(float scalar) -> Vector4 &
    {
        x += scalar;
        y += scalar;
        z += scalar;
        w += scalar;
        return *this;
    }

    inline auto Vector4::operator-() const -> Vector4
    {
        Vector4 result;
        result.x = -x;
        result.y = -y;
        result.z = -z;
        result.w = -w;
        return result;
    }

    inline auto Vector4::operator-(float scalar) const -> Vector4
    {
        auto result(*this);
        result -= scalar;
        return result;
    }

    inline auto Vector4::operator-(const Vector4 &v) const -> Vector4
    {
        auto result(*this);
        result -= v;
        return result;
    }

    inline auto Vector4::operator-=(float scalar) -> Vector4 &
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        w -= scalar;
        return *this;
    }

    inline auto Vector4::operator-=(const Vector4 &v) -> Vector4 &
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }

    inline auto Vector4::operator*(float scalar) const -> Vector4
    {
        auto result(*this);
        result *= scalar;
        return result;
    }

    inline auto operator*(float scalar, const Vector4 &v) -> Vector4
    {
        return {v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar};
    }

    inline auto Vector4::operator*=(float scalar) -> Vector4 &
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    inline auto Vector4::operator/(const float scalar) const -> Vector4
    {
        return {this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar};
    }

    inline auto Vector4::operator/=(const float scalar) -> Vector4 &
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    inline bool Vector4::operator<(const Vector4 &v) const
    {
        if (x == v.x)
        {
            if (y == v.y)
            {
                if (z == v.z)
                    return w < v.w;
                return z < v.z;
            }
            return y < v.y;
        }
        return x < v.x;
    }

    inline bool Vector4::operator==(const Vector4 &v) const
    {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    inline bool Vector4::operator!=(const Vector4 &v) const
    {
        return x != v.x || y != v.y || z != v.z || w != v.w;
    }
}
