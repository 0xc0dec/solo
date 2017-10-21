/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloRadian.h"
#include "SoloMath.h"
#include "SoloHash.h"

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

        auto distance(const Vector2 &v) const -> float;
        auto distanceSquared(const Vector2 &v) const -> float;
        auto length() const -> float;
        auto lengthSquared() const -> float;
        auto normalized() const -> Vector2;
        void normalize();
        auto angle(const Vector2 &v) const -> Radian;
        void clamp(const Vector2 &min, const Vector2 &max);
        auto dot(const Vector2 &v) const -> float;

        bool operator==(const Vector2 &v) const;

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

    inline bool Vector2::operator==(const Vector2 &v) const
    {
        return math::areEqual(x, v.x) && math::areEqual(y, v.y);
    }

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

namespace std
{
    template <>
	struct hash<solo::Vector2>
	{
		size_t operator()(const solo::Vector2 &v) const
        {
            size_t seed = 0;
            const hash<float> hasher;
            solo::combineHash(seed, hasher(v.x));
            solo::combineHash(seed, hasher(v.y));
		    return seed;
        }
	};
}
