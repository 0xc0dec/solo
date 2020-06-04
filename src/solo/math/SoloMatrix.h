/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloVector3.h"

namespace solo
{
    struct Radians;
    class Ray;
    class Quaternion;

    class Matrix
    {
    public:
        Matrix();
        Matrix(const Matrix &copy);
        Matrix(const glm::mat4x4 &data);
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
        static auto createTranslation(const Vector3 &translation) -> Matrix;

        auto columns() const -> const float*;

        bool isIdentity() const;
        auto getDeterminant() const -> float;
        void invert();
        void transpose();

        auto scale() const -> Vector3;
        auto rotation() const -> Quaternion;
        auto translation() const -> Vector3;

        auto upVector() const -> Vector3 { return {data_[1][0], data_[1][1], data_[1][2]}; }
        auto downVector() const -> Vector3 { return {-data_[1][0], -data_[1][1], -data_[1][2]}; }
        auto leftVector() const -> Vector3 { return {-data_[0][0], -data_[0][1], -data_[0][2]}; }
        auto rightVector() const -> Vector3 { return {data_[0][0], data_[0][1], data_[0][2]}; }
        auto forwardVector() const -> Vector3 { return {-data_[2][0], -data_[2][1], -data_[2][2]}; }
        auto backVector() const -> Vector3 { return {data_[2][0], data_[2][1], data_[2][2]}; }

        void rotateByQuaternion(const Quaternion &q);
        void rotateByAxisAngle(const Vector3 &axis, const Radians &angle);

        void scaleByScalar(float value);
        void scaleByVector(const Vector3 &s);

        void translate(const Vector3 &t);

        auto transformPoint(const Vector3 &point) const -> Vector3;
        auto transformDirection(const Vector3 &dir) const -> Vector3;
        auto transformRay(const Ray &ray) const -> Ray;

        void decompose(Vector3 &scale, Quaternion &rotation, Vector3 &translation) const;

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

    private:
        glm::mat4x4 data_;
    };
}
