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
    class Vector4 final
    {
    public:
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;

        Vector4() {}
        Vector4(float x, float y, float z, float w);

        bool isZero() const;
        bool isUnit() const;

        auto distance(const Vector4 &v) const -> float;
        auto distanceSquared(const Vector4 &v) const -> float;
        auto length() const -> float;
        auto lengthSquared() const -> float;
        auto normalized() const -> Vector4;
        void normalize();
        auto angle(const Vector4 &v) -> Radian;
        void clamp(const Vector4 &min, const Vector4 &max);
        auto dot(const Vector4 &v) const -> float;

        bool operator==(const Vector4 &v) const;

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

    inline bool Vector4::operator==(const Vector4 &v) const
    {
        return math::areEqual(x, v.x) && math::areEqual(y, v.y) &&
            math::areEqual(z, v.z) && math::areEqual(w, v.w);
    }

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

namespace std
{
    template <>
	struct hash<solo::Vector4>
	{
		size_t operator()(const solo::Vector4 &v) const
        {
            size_t seed = 0;
            const hash<float> hasher;
            solo::combineHash(seed, hasher(v.x));
            solo::combineHash(seed, hasher(v.y));
            solo::combineHash(seed, hasher(v.z));
            solo::combineHash(seed, hasher(v.w));
		    return seed;
        }
	};
}
