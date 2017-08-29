/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
}
