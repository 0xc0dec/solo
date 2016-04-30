#pragma once

#include "SoloRadian.h"


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

        static auto zero() -> Vector3;
        static auto unit() -> Vector3;
        static auto unitX() -> Vector3;
        static auto unitY() -> Vector3;
        static auto unitZ() -> Vector3;

        bool isZero() const;
        bool isUnit() const;

        static auto angle(const Vector3& v1, const Vector3& v2) -> Radian;

        void clamp(const Vector3& min, const Vector3& max);

        static auto cross(const Vector3& v1, const Vector3& v2) -> Vector3;

        auto distance(const Vector3& v) const -> float;
        auto distanceSquared(const Vector3& v) const -> float;

        auto dot(const Vector3& v) const -> float;
        static auto dot(const Vector3& v1, const Vector3& v2) -> float;

        auto length() const -> float;
        auto lengthSquared() const -> float;

        auto normalized() const -> Vector3;
        void normalize();

        auto operator+(float scalar) const -> Vector3;
        auto operator+(const Vector3& v) const -> Vector3;
        auto operator+=(const Vector3& v) -> Vector3&;
        auto operator+=(float scalar) -> Vector3&;

        auto operator-() const -> Vector3;
        auto operator-(float scalar) const -> Vector3;
        auto operator-(const Vector3& v) const -> Vector3;
        auto operator-=(float scalar) -> Vector3&;
        auto operator-=(const Vector3& v) -> Vector3&;

        auto operator*(float scalar) const -> Vector3;
        auto operator*=(float scalar) -> Vector3&;

        auto operator/(float scalar) const -> Vector3;
        auto operator/=(float scalar) -> Vector3&;

        bool operator<(const Vector3& v) const;

        bool operator==(const Vector3& v) const;
        bool operator!=(const Vector3& v) const;
    };


    inline auto Vector3::operator+(const Vector3& v) const -> Vector3
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

    inline auto operator+(float scalar, const Vector3& v) -> Vector3
    {
        return Vector3(v.x + scalar, v.y + scalar, v.z + scalar);
    }

    inline auto Vector3::operator+=(const Vector3& v) -> Vector3&
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    inline auto Vector3::operator+=(float scalar) -> Vector3&
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

    inline auto Vector3::operator-(const Vector3& v) const -> Vector3
    {
        auto result(*this);
        result -= v;
        return result;
    }

    inline auto Vector3::operator-=(const Vector3& v) -> Vector3&
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    inline auto Vector3::operator-=(float scalar) -> Vector3&
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

    inline auto operator*(float scalar, const Vector3& v) -> Vector3
    {
        return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
    }

    inline auto Vector3::operator*=(float scalar) -> Vector3&
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    inline auto Vector3::operator/(const float scalar) const -> Vector3
    {
        return Vector3(this->x / scalar, this->y / scalar, this->z / scalar);
    }

    inline auto Vector3::operator/=(float scalar) -> Vector3&
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    inline bool Vector3::operator<(const Vector3& v) const
    {
        if (x == v.x)
        {
            if (y == v.y)
                return z < v.z;
            return y < v.y;
        }
        return x < v.x;
    }

    inline bool Vector3::operator==(const Vector3& v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }

    inline bool Vector3::operator!=(const Vector3& v) const
    {
        return x != v.x || y != v.y || z != v.z;
    }
}
