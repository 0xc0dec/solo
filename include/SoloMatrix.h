/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloVector3.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <stddef.h> // for size_t

namespace solo
{
    struct Radian;
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
        static auto getSize() -> size_t;

        static auto createLookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up) -> Matrix;
        static auto createLookAt2(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up) -> Matrix;
        static auto createPerspective(const Radian &fieldOfView, float aspectRatio, float znear, float zfar) -> Matrix;
        static auto createPerspective2(float fieldOfView, float aspectRatio, float znear, float zfar) -> Matrix;
        static auto createOrthographic(float width, float height, float near, float far) -> Matrix;
        static auto createScale(const Vector3 &scale) -> Matrix;
        static auto createScale2(const glm::vec3 &scale) -> Matrix;
        static auto createRotationFromQuaternion(const Quaternion &quat) -> Matrix;
        static auto createRotationFromQuaternion2(const glm::quat &quat) -> Matrix;
        static auto createRotationFromAxisAngle(const Vector3 &axis, const Radian &angle) -> Matrix;
        static auto createRotationFromAxisAngle2(const glm::vec3 &axis, float angle) -> Matrix;
        static auto createRotationX(const Radian &angle) -> Matrix;
        static auto createRotationX2(float angle) -> Matrix;
        static auto createRotationY(const Radian &angle) -> Matrix;
        static auto createRotationY2(float angle) -> Matrix;
        static auto createRotationZ(const Radian &angle) -> Matrix;
        static auto createRotationZ2(float angle) -> Matrix;
        static auto createTranslation(const Vector3 &translation) -> Matrix;
        static auto createTranslation2(const glm::vec3 &translation) -> Matrix;

        bool isIdentity() const;
        auto getDeterminant() const -> float;
        bool invert();
        void transpose();

        auto getScale() const -> Vector3;
        auto getScale2() const -> glm::vec3;
        auto getRotation() const -> Quaternion;
        auto getRotation2() const -> glm::quat;
        auto getTranslation() const -> Vector3;
        auto getTranslation2() const -> glm::vec3;

        auto getUpVector() const -> Vector3;
        auto getUpVector2() const -> glm::vec3;
        auto getDownVector() const -> Vector3;
        auto getDownVector2() const -> glm::vec3;
        auto getLeftVector() const -> Vector3;
        auto getLeftVector2() const -> glm::vec3;
        auto getRightVector() const -> Vector3;
        auto getRightVector2() const -> glm::vec3;
        auto getForwardVector() const -> Vector3;
        auto getForwardVector2() const -> glm::vec3;
        auto getBackVector() const -> Vector3;
        auto getBackVector2() const -> glm::vec3;

        void rotateByQuaternion(const Quaternion &q);
        void rotateByQuaternion2(const glm::quat &q);
        void rotateByAxisAngle(const Vector3 &axis, const Radian &angle);
        void rotateByAxisAngle2(const glm::vec3 &axis, float angle);
        void rotateX(const Radian &angle);
        void rotateX2(float angle);
        void rotateY(const Radian &angle);
        void rotateY2(float angle);
        void rotateZ(const Radian &angle);
        void rotateZ2(float angle);

        void scaleByScalar(float value);
        void scaleByVector(const Vector3 &s);
        void scaleByVector2(const glm::vec3 &s);

        void translate(const Vector3 &t);
        void translate2(const glm::vec3 &t);

        auto transformPoint(const Vector3 &point) const -> Vector3;
        auto transformPoint2(const glm::vec3 &point) const -> glm::vec3;
        auto transformDirection(const Vector3 &dir) const -> Vector3;
        auto transformDirection2(const glm::vec3 &dir) const -> glm::vec3;
        auto transformRay(const Ray &ray) -> Ray;

        bool decompose(Vector3 *scale, Quaternion *rotation, Vector3 *translation) const;
        bool decompose2(glm::vec3 *scale, glm::quat *rotation, glm::vec3 *translation) const;

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

    inline auto Matrix::getUpVector() const -> Vector3
    {
        return {m[4], m[5], m[6]};
    }

    inline auto Matrix::getUpVector2() const -> glm::vec3
    {
        return {m[4], m[5], m[6]};
    }

    inline auto Matrix::getDownVector() const -> Vector3
    {
        return {-m[4], -m[5], -m[6]};
    }

    inline auto Matrix::getDownVector2() const -> glm::vec3
    {
        return {-m[4], -m[5], -m[6]};
    }

    inline auto Matrix::getLeftVector() const -> Vector3
    {
        return {-m[0], -m[1], -m[2]};
    }

    inline auto Matrix::getLeftVector2() const -> glm::vec3
    {
        return {-m[0], -m[1], -m[2]};
    }

    inline auto Matrix::getRightVector() const -> Vector3
    {
        return {m[0], m[1], m[2]};
    }

    inline auto Matrix::getRightVector2() const -> glm::vec3
    {
        return {m[0], m[1], m[2]};
    }

    inline auto Matrix::getForwardVector() const -> Vector3
    {
        return {-m[8], -m[9], -m[10]};
    }

    inline auto Matrix::getForwardVector2() const -> glm::vec3
    {
        return {-m[8], -m[9], -m[10]};
    }

    inline auto Matrix::getBackVector() const -> Vector3
    {
        return {m[8], m[9], m[10]};
    }

    inline auto Matrix::getBackVector2() const -> glm::vec3
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

    inline void Matrix::rotateByQuaternion2(const glm::quat &q)
    {
        *this *= createRotationFromQuaternion2(q);
    }

    inline void Matrix::rotateByAxisAngle(const Vector3 &axis, const Radian &angle)
    {
        *this *= createRotationFromAxisAngle(axis, angle);
    }

    inline void Matrix::rotateByAxisAngle2(const glm::vec3 &axis, float angle)
    {
        *this *= createRotationFromAxisAngle2(axis, angle);
    }

    inline void Matrix::rotateX(const Radian &angle)
    {
        *this *= createRotationX(angle);
    }

    inline void Matrix::rotateX2(float angle)
    {
        *this *= createRotationX2(angle);
    }

    inline void Matrix::rotateY(const Radian &angle)
    {
        *this *= createRotationY(angle);
    }

    inline void Matrix::rotateY2(float angle)
    {
        *this *= createRotationY2(angle);
    }

    inline void Matrix::rotateZ(const Radian &angle)
    {
        *this *= createRotationZ(angle);
    }

    inline void Matrix::rotateZ2(float angle)
    {
        *this *= createRotationZ2(angle);
    }

    inline void Matrix::scaleByVector(const Vector3 &s)
    {
        *this *= createScale(s);
    }

    inline void Matrix::scaleByVector2(const glm::vec3 &s)
    {
        *this *= createScale2(s);
    }

    inline void Matrix::translate(const Vector3 &t)
    {
        *this *= createTranslation(t);
    }

    inline void Matrix::translate2(const glm::vec3 &t)
    {
        *this *= createTranslation2(t);
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
            v.x * m.m[0] + v.y * m.m[4] + v.z * m.m[8],
            v.x * m.m[1] + v.y * m.m[5] + v.z * m.m[9],
            v.x * m.m[2] + v.y * m.m[6] + v.z * m.m[10]
        );
        return v;
    }

    inline auto operator*(const Matrix &m, const Vector3 &v) -> Vector3
    {
        return {
            v.x * m.m[0] + v.y * m.m[4] + v.z * m.m[8],
            v.x * m.m[1] + v.y * m.m[5] + v.z * m.m[9],
            v.x * m.m[2] + v.y * m.m[6] + v.z * m.m[10]
        };
    }
}
