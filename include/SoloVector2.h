/*
    Copyright (c) Aleksey Fedotov
    MIT license
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

        bool isZero() const;
        bool isUnit() const;

        void clamp(const Vector2 &min, const Vector2 &max);
        auto distance(const Vector2 &v) const -> float;
        auto distanceSquared(const Vector2 &v) const -> float;
        auto dot(const Vector2 &v) const -> float;
        auto angle(const Vector2 &v) const -> Radian;
        auto length() const -> float;
        auto lengthSquared() const -> float;
        auto normalized() const -> Vector2;
        void normalize();

        auto operator+(const Vector2 &v) const -> Vector2;
        auto operator+=(float scalar) -> Vector2&;
        auto operator+=(const Vector2 &v) -> Vector2&;

        auto operator-() const -> Vector2;
        auto operator-(const Vector2 &v) const -> Vector2;
        auto operator-=(float scalar) -> Vector2&;
        auto operator-=(const Vector2 &v) -> Vector2&;

        auto operator*(float scalar) const -> Vector2;
        auto operator*=(float scalar) -> Vector2&;

        auto operator/(float scalar) const -> Vector2;
        auto operator/=(float scalar) -> Vector2&;
    };

    inline auto Vector2::operator+(const Vector2 &v) const -> Vector2
    {
        auto result(*this);
        result += v;
        return result;
    }

    inline auto operator+(float scalar, const Vector2 &v) -> Vector2
    {
        return {v.x + scalar, v.y + scalar};
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
        return {v.x * scalar, v.y * scalar};
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
}
