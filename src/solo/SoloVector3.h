/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloRadian.h"
#include "SoloMath.h"

namespace solo
{
    class Vector3 final
    {
    public:
        float x = 0;
        float y = 0;
        float z = 0;

        Vector3() {}
        Vector3(float all);
        Vector3(float x, float y, float z);

        bool isZero() const;
        bool isUnit() const;

        auto distance(const Vector3 &v) const -> float;
        auto distanceSquared(const Vector3 &v) const -> float;
        auto length() const -> float;
        auto lengthSquared() const -> float;
        auto normalized() const -> Vector3;
        void normalize();
        auto angle(const Vector3 &v) -> Radian;
        void clamp(const Vector3 &min, const Vector3 &max);
        auto dot(const Vector3 &v) const -> float;
        auto cross(const Vector3 &v) -> Vector3;

        bool operator==(const Vector3 &v) const;

        auto operator+(float scalar) const -> Vector3;
        auto operator+(const Vector3 &v) const -> Vector3;
        auto operator+=(const Vector3 &v) -> Vector3&;
        auto operator+=(float scalar) -> Vector3&;

        auto operator-() const -> Vector3;
        auto operator-(float scalar) const -> Vector3;
        auto operator-(const Vector3 &v) const -> Vector3;
        auto operator-=(float scalar) -> Vector3&;
        auto operator-=(const Vector3 &v) -> Vector3&;

        auto operator*(float scalar) const -> Vector3;
        auto operator*=(float scalar) -> Vector3&;

        auto operator/(float scalar) const -> Vector3;
        auto operator/=(float scalar) -> Vector3&;
    };

    inline bool Vector3::operator==(const Vector3 &v) const
    {
        return math::areEqual(x, v.x) && math::areEqual(y, v.y) && math::areEqual(z, v.z);
    }

    inline auto Vector3::operator+(const Vector3 &v) const -> Vector3
    {
        auto result(*this);
        result += v;
        return result;
    }

    inline auto Vector3::operator+(float scalar) const -> Vector3
    {
        auto result(*this);
        result += scalar;
        return result;
    }

    inline auto operator+(float scalar, const Vector3 &v) -> Vector3
    {
        return {v.x + scalar, v.y + scalar, v.z + scalar};
    }

    inline auto Vector3::operator+=(const Vector3 &v) -> Vector3 &
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    inline auto Vector3::operator+=(float scalar) -> Vector3 &
    {
        x += scalar;
        y += scalar;
        z += scalar;
        return *this;
    }

    inline auto Vector3::operator-() const -> Vector3
    {
        Vector3 result;
        result.x = -x;
        result.y = -y;
        result.z = -z;
        return result;
    }

    inline auto Vector3::operator-(float scalar) const -> Vector3
    {
        auto result(*this);
        result -= scalar;
        return result;
    }

    inline auto Vector3::operator-(const Vector3 &v) const -> Vector3
    {
        auto result(*this);
        result -= v;
        return result;
    }

    inline auto Vector3::operator-=(const Vector3 &v) -> Vector3 &
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    inline auto Vector3::operator-=(float scalar) -> Vector3 &
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        return *this;
    }

    inline auto Vector3::operator*(float scalar) const -> Vector3
    {
        auto result(*this);
        result *= scalar;
        return result;
    }

    inline auto operator*(float scalar, const Vector3 &v) -> Vector3
    {
        return {v.x * scalar, v.y * scalar, v.z * scalar};
    }

    inline auto Vector3::operator*=(float scalar) -> Vector3 &
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    inline auto Vector3::operator/(const float scalar) const -> Vector3
    {
        return {this->x / scalar, this->y / scalar, this->z / scalar};
    }

    inline auto Vector3::operator/=(float scalar) -> Vector3 &
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
}
