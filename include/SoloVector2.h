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
    class Vector2 final
    {
    public:
        float x = 0;
        float y = 0;

        Vector2() {}
        Vector2(float x, float y);

        static auto zero() -> Vector2;
        static auto unit() -> Vector2;
        static auto unitX() -> Vector2;
        static auto unitY() -> Vector2;

        bool isZero() const;
        bool isUnit() const;

        static auto angle(const Vector2 &v1, const Vector2 &v2) -> Radian;

        void clamp(const Vector2 &min, const Vector2 &max);

        auto distance(const Vector2 &v) const -> float;
        auto distanceSquared(const Vector2 &v) const -> float;

        auto dot(const Vector2 &v) const -> float;
        static auto dot(const Vector2 &v1, const Vector2 &v2) -> float;

        auto length() const -> float;
        auto lengthSquared() const -> float;

        auto normalized() const -> Vector2;
        void normalize();

        auto operator+(float scalar) const -> Vector2;
        auto operator+(const Vector2 &v) const -> Vector2;
        auto operator+=(float scalar) -> Vector2&;
        auto operator+=(const Vector2 &v) -> Vector2&;

        auto operator-() const -> Vector2;
        auto operator-(float scalar) const -> Vector2;
        auto operator-(const Vector2 &v) const -> Vector2;
        auto operator-=(float scalar) -> Vector2&;
        auto operator-=(const Vector2 &v) -> Vector2&;

        auto operator*(float scalar) const -> Vector2;
        auto operator*=(float scalar) -> Vector2&;

        auto operator/(float scalar) const -> Vector2;
        auto operator/=(float scalar) -> Vector2&;

        bool operator<(const Vector2 &v) const;

        bool operator==(const Vector2 &v) const;
        bool operator!=(const Vector2 &v) const;
    };

    inline auto Vector2::operator+(float scalar) const -> Vector2
    {
        auto result(*this);
        result += scalar;
        return result;
    }

    inline auto Vector2::operator+(const Vector2 &v) const -> Vector2
    {
        auto result(*this);
        result += v;
        return result;
    }

    inline auto operator+(float scalar, const Vector2 &v) -> Vector2
    {
        return Vector2(v.x + scalar, v.y + scalar);
    }

    inline auto Vector2::operator+=(float scalar) -> Vector2 &
    {
        x += scalar;
        y += scalar;
        return *this;
    }

    inline auto Vector2::operator+=(const Vector2 &v) -> Vector2 &
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    inline auto Vector2::operator-() const -> Vector2
    {
        auto result(*this);
        result.x = -result.x;
        result.y = -result.y;
        return result;
    }

    inline auto Vector2::operator-(float scalar) const -> Vector2
    {
        auto result(*this);
        result -= scalar;
        return result;
    }

    inline auto Vector2::operator-(const Vector2 &v) const -> Vector2
    {
        auto result(*this);
        result -= v;
        return result;
    }

    inline auto Vector2::operator-=(float scalar) -> Vector2 &
    {
        x -= scalar;
        y -= scalar;
        return *this;
    }

    inline auto Vector2::operator-=(const Vector2 &v) -> Vector2 &
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    inline auto Vector2::operator*(float scalar) const -> Vector2
    {
        auto result(*this);
        result *= scalar;
        return result;
    }

    inline auto operator*(float scalar, const Vector2 &v) -> Vector2
    {
        return Vector2(v.x * scalar, v.y * scalar);
    }

    inline auto Vector2::operator*=(float scalar) -> Vector2 &
    {
        this->x *= scalar;
        this->y *= scalar;
        return *this;
    }

    inline auto Vector2::operator/(const float scalar) const -> Vector2
    {
        auto result(*this);
        result /= scalar;
        return result;
    }

    inline auto Vector2::operator/=(float scalar) -> Vector2 &
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    inline bool Vector2::operator<(const Vector2 &v) const
    {
        if (x == v.x)
            return y < v.y;
        return x < v.x;
    }

    inline bool Vector2::operator==(const Vector2 &v) const
    {
        return x == v.x && y == v.y;
    }

    inline bool Vector2::operator!=(const Vector2 &v) const
    {
        return x != v.x || y != v.y;
    }
}
