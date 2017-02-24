/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloVector3.h"
#include <stddef.h> // for size_t


namespace solo
{
    class Plane;
    class Quaternion;
    struct Radian;

    class Matrix
    {
    public:
        // column-major order
        float m[16];

        Matrix();
        Matrix(const Matrix &copy);
        Matrix(float m11, float m12, float m13, float m14,
               float m21, float m22, float m23, float m24,
               float m31, float m32, float m33, float m34,
               float m41, float m42, float m43, float m44);

        static auto identity() -> Matrix;

        static auto getSize() -> size_t;

        bool isIdentity() const;
        auto getDeterminant() const -> float;
        bool invert();
        void transpose();
        void resetToIdentity();
        void resetToZero();

        auto operator+(float scalar) const -> Matrix;
        auto operator+(const Matrix &m) const -> Matrix;
        auto operator+=(float scalar) -> Matrix&;
        auto operator+=(const Matrix &m) -> Matrix&;

        auto operator-() const -> Matrix;
        auto operator-(float scalar) const -> Matrix;
        auto operator-(const Matrix &m) const -> Matrix;
        auto operator-=(float scalar) -> Matrix&;
        auto operator-=(const Matrix &m) -> Matrix&;

        auto operator*(float scalar) const -> Matrix;
        auto operator*(const Matrix &m) const -> Matrix;
        auto operator*=(float scalar) -> Matrix&;
        auto operator*=(const Matrix &m) -> Matrix&;
    };

    inline auto Matrix::getSize() -> size_t
    {
        return sizeof(float) * 16;
    }

    inline auto Matrix::operator+(float scalar) const -> Matrix
    {
        auto result(*this);
        result += scalar;
        return result;
    }

    inline auto Matrix::operator+(const Matrix &m) const -> Matrix
    {
        auto result(*this);
        result += m;
        return result;
    }

    inline auto Matrix::operator-(float scalar) const -> Matrix
    {
        auto result(*this);
        result -= scalar;
        return result;
    }

    inline auto Matrix::operator-(const Matrix &m) const -> Matrix
    {
        auto result(*this);
        result -= m;
        return result;
    }

    inline auto Matrix::operator*(float scalar) const -> Matrix
    {
        auto result(*this);
        result *= scalar;
        return result;
    }

    inline auto Matrix::operator*(const Matrix &m) const -> Matrix
    {
        auto result(*this);
        return result *= m;
    }

    inline auto operator*=(Vector3 &v, const Matrix &m) -> Vector3 &
    {
        v = Vector3(
            v.x *m.m[0] + v.y *m.m[4] + v.z *m.m[8],
            v.x *m.m[1] + v.y *m.m[5] + v.z *m.m[9],
            v.x *m.m[2] + v.y *m.m[6] + v.z *m.m[10]
        );
        return v;
    }

    inline auto operator*(const Matrix &m, const Vector3 &v) -> Vector3
    {
        return{
            v.x *m.m[0] + v.y *m.m[4] + v.z *m.m[8],
            v.x *m.m[1] + v.y *m.m[5] + v.z *m.m[9],
            v.x *m.m[2] + v.y *m.m[6] + v.z *m.m[10]
        };
    }
}
