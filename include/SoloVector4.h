#pragma once

namespace solo
{
    struct Vector4
    {
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;

        Vector4() {}
        Vector4(float x, float y, float z, float w);

        static Vector4 zero();
        static Vector4 unit();
        static Vector4 unitX();
        static Vector4 unitY();
        static Vector4 unitZ();
        static Vector4 unitW();

        bool isZero() const;
        bool isUnit() const;

        // in radians
        static float angle(const Vector4 &v1, const Vector4 &v2);

        void clamp(const Vector4 &min, const Vector4 &max);

        float distance(const Vector4 &v) const;
        float distanceSquared(const Vector4 &v) const;

        float dot(const Vector4 &v) const;
        static float dot(const Vector4 &v1, const Vector4 &v2);

        float length() const;
        float lengthSquared() const;

        Vector4 normalized() const;
        void normalize();

        Vector4 operator+(float scalar) const;
        Vector4 operator+(const Vector4 &v) const;
        Vector4 &operator+=(float scalar);
        Vector4 &operator+=(const Vector4 &v);

        Vector4 operator-() const;
        Vector4 operator-(float scalar) const;
        Vector4 operator-(const Vector4 &v) const;
        Vector4 &operator-=(float scalar);
        Vector4 &operator-=(const Vector4 &v);

        Vector4 operator*(float x) const;
        Vector4 &operator*=(float x);

        Vector4 operator/(float x) const;
        Vector4 operator/=(float x);

        bool operator<(const Vector4 &v) const;

        bool operator==(const Vector4 &v) const;
        bool operator!=(const Vector4 &v) const;
    };

    inline Vector4 Vector4::operator+(float scalar) const
    {
        auto result(*this);
        result += scalar;
        return result;
    }

    inline Vector4 Vector4::operator+(const Vector4 &v) const
    {
        auto result(*this);
        result += v;
        return result;
    }

    inline Vector4 operator+(float scalar, const Vector4 &v)
    {
        return Vector4(v.x + scalar, v.y + scalar, v.z + scalar, v.w + scalar);
    }

    inline Vector4 &Vector4::operator+=(const Vector4 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }

    inline Vector4 &Vector4::operator+=(float scalar)
    {
        x += scalar;
        y += scalar;
        z += scalar;
        w += scalar;
        return *this;
    }

    inline Vector4 Vector4::operator-() const
    {
        Vector4 result;
        result.x = -x;
        result.y = -y;
        result.z = -z;
        result.w = -w;
        return result;
    }

    inline Vector4 Vector4::operator-(float scalar) const
    {
        auto result(*this);
        result -= scalar;
        return result;
    }

    inline Vector4 Vector4::operator-(const Vector4 &v) const
    {
        auto result(*this);
        result -= v;
        return result;
    }

    inline Vector4 &Vector4::operator-=(float scalar)
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        w -= scalar;
        return *this;
    }

    inline Vector4 &Vector4::operator-=(const Vector4 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }

    inline Vector4 Vector4::operator*(float scalar) const
    {
        auto result(*this);
        result *= scalar;
        return result;
    }

    inline Vector4 operator*(float scalar, const Vector4 &v)
    {
        return Vector4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
    }

    inline Vector4 &Vector4::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    inline Vector4 Vector4::operator/(const float scalar) const
    {
        return Vector4(this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar);
    }

    inline Vector4 Vector4::operator/=(const float scalar)
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
