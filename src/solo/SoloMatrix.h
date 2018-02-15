/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloVector3.h"
#include <stddef.h> // for size_t

namespace solo
{
    struct Radians;
    class Ray;
    class Quaternion;

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

        static auto createLookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up) -> Matrix;
        static auto createPerspective(const Radians &fieldOfView, float aspectRatio, float znear, float zfar) -> Matrix;
        static auto createOrthographic(float width, float height, float near, float far) -> Matrix;
        static auto createScale(const Vector3 &scale) -> Matrix;
        static auto createRotationFromQuaternion(const Quaternion &q) -> Matrix;
        static auto createRotationFromAxisAngle(const Vector3 &axis, const Radians &angle) -> Matrix;
        static auto createRotationX(const Radians &angle) -> Matrix;
        static auto createRotationY(const Radians &angle) -> Matrix;
        static auto createRotationZ(const Radians &angle) -> Matrix;
        static auto createTranslation(const Vector3 &translation) -> Matrix;

        bool isIdentity() const;
        auto getDeterminant() const -> float;
        bool invert();
        void transpose();

        auto getScale() const -> Vector3;
        auto getRotation() const -> Quaternion;
        auto getTranslation() const -> Vector3;

        auto getUpVector() const -> Vector3;
        auto getDownVector() const -> Vector3;
        auto getLeftVector() const -> Vector3;
        auto getRightVector() const -> Vector3;
        auto getForwardVector() const -> Vector3;
        auto getBackVector() const -> Vector3;

        void rotateByQuaternion(const Quaternion &q);
        void rotateByAxisAngle(const Vector3 &axis, const Radians &angle);
        void rotateX(const Radians &angle);
        void rotateY(const Radians &angle);
        void rotateZ(const Radians &angle);

        void scaleByScalar(float value);
        void scaleByVector(const Vector3 &s);

        void translate(const Vector3 &t);

        auto transformPoint(const Vector3 &point) const -> Vector3;
        auto transformDirection(const Vector3 &dir) const -> Vector3;
        auto transformRay(const Ray &ray) const -> Ray;

        bool decompose(Vector3 *scale, Quaternion *rotation, Vector3 *translation) const;

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

    inline auto Matrix::getUpVector() const -> Vector3
    {
        return {m[4], m[5], m[6]};
    }

    inline auto Matrix::getDownVector() const -> Vector3
    {
        return {-m[4], -m[5], -m[6]};
    }

    inline auto Matrix::getLeftVector() const -> Vector3
    {
        return {-m[0], -m[1], -m[2]};
    }

    inline auto Matrix::getRightVector() const -> Vector3
    {
        return {m[0], m[1], m[2]};
    }

    inline auto Matrix::getForwardVector() const -> Vector3
    {
        return {-m[8], -m[9], -m[10]};
    }

    inline auto Matrix::getBackVector() const -> Vector3
    {
        return {m[8], m[9], m[10]};
    }

    inline void Matrix::scaleByScalar(float value)
    {
        scaleByVector({value, value, value});
    }

    inline void Matrix::rotateByQuaternion(const Quaternion &q)
    {
        *this *= createRotationFromQuaternion(q);
    }

    inline void Matrix::rotateByAxisAngle(const Vector3 &axis, const Radians &angle)
    {
        *this *= createRotationFromAxisAngle(axis, angle);
    }

    inline void Matrix::rotateX(const Radians &angle)
    {
        *this *= createRotationX(angle);
    }

    inline void Matrix::rotateY(const Radians &angle)
    {
        *this *= createRotationY(angle);
    }

    inline void Matrix::rotateZ(const Radians &angle)
    {
        *this *= createRotationZ(angle);
    }

    inline void Matrix::scaleByVector(const Vector3 &s)
    {
        *this *= createScale(s);
    }

    inline void Matrix::translate(const Vector3 &t)
    {
        *this *= createTranslation(t);
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

    inline auto operator*=(Vector3 &v, const Matrix &m) -> Vector3&
    {
        v = Vector3(
            v.x() * m.m[0] + v.y() * m.m[4] + v.z() * m.m[8],
            v.x() * m.m[1] + v.y() * m.m[5] + v.z() * m.m[9],
            v.x() * m.m[2] + v.y() * m.m[6] + v.z() * m.m[10]
        );
        return v;
    }

    inline auto operator*(const Matrix &m, const Vector3 &v) -> Vector3
    {
        return {
            v.x() * m.m[0] + v.y() * m.m[4] + v.z() * m.m[8],
            v.x() * m.m[1] + v.y() * m.m[5] + v.z() * m.m[9],
            v.x() * m.m[2] + v.y() * m.m[6] + v.z() * m.m[10]
        };
    }
}
