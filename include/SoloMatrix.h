#pragma once

#include "SoloVector3.h"


namespace solo
{
    class Plane;
    class Quaternion;
    struct Radian;

    class Matrix final
    {
    public:
        // column-major order
        float m[16];

        Matrix();
        Matrix(float m11, float m12, float m13, float m14,
               float m21, float m22, float m23, float m24,
               float m31, float m32, float m33, float m34,
               float m41, float m42, float m43, float m44);
        Matrix(const Matrix& copy);

        static auto identity() -> Matrix;
        static auto zero() -> Matrix;

        bool isIdentity() const;

        static auto createLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up) -> Matrix;
        static auto createPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane) -> Matrix;
        static auto createOrthographic(float width, float height, float zNearPlane, float zFarPlane) -> Matrix;
        static auto createOrthographicOffCenter(float left, float right, float bottom, float top, float near, float far) -> Matrix;
        static auto createReflection(const Plane& plane) -> Matrix;
        static auto createScale(const Vector3& scale) -> Matrix;
        static auto createRotationFromQuaternion(const Quaternion& quat) -> Matrix;
        static auto createRotationFromAxisAngle(const Vector3& axis, const Radian& angle) -> Matrix;
        static auto createRotationX(const Radian& angle) -> Matrix;
        static auto createRotationY(const Radian& angle) -> Matrix;
        static auto createRotationZ(const Radian& angle) -> Matrix;
        static auto createTranslation(const Vector3& translation) -> Matrix;

        bool decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const;

        auto getDeterminant() const -> float;

        auto getScale() const -> Vector3;
        auto getRotation() const -> Quaternion;
        auto getTranslation() const -> Vector3;

        auto getUpVector() const -> Vector3;
        auto getDownVector() const -> Vector3;
        auto getLeftVector() const -> Vector3;
        auto getRightVector() const -> Vector3;
        auto getForwardVector() const -> Vector3;
        auto getBackVector() const -> Vector3;

        bool invert();
        void transpose();

        void rotateByQuaternion(const Quaternion& q);
        void rotateByAxisAngle(const Vector3& axis, const Radian& angle);
        void rotateX(const Radian& angle);
        void rotateY(const Radian& angle);
        void rotateZ(const Radian& angle);

        void scaleByScalar(float value);
        void scaleByVector(const Vector3& s);

        void translate(const Vector3& t);

        void setIdentity();
        void setZero();

        auto transformPoint(const Vector3& point) const -> Vector3;
        auto transformDirection(const Vector3& dir) const -> Vector3;

        auto operator+(float scalar) const -> Matrix;
        auto operator+(const Matrix& m) const -> Matrix;
        auto operator+=(float scalar) -> Matrix&;
        auto operator+=(const Matrix& m) -> Matrix&;

        auto operator-() const -> Matrix;
        auto operator-(float scalar) const -> Matrix;
        auto operator-(const Matrix& m) const -> Matrix;
        auto operator-=(float scalar) -> Matrix&;
        auto operator-=(const Matrix& m) -> Matrix&;

        auto operator*(float scalar) const -> Matrix;
        auto operator*(const Matrix& m) const -> Matrix;
        auto operator*=(float scalar) -> Matrix&;
        auto operator*=(const Matrix& m) -> Matrix&;
    };

    inline auto Matrix::operator+(float scalar) const -> Matrix
    {
        auto result(*this);
        result += scalar;
        return result;
    }

    inline auto Matrix::operator+(const Matrix& m) const -> Matrix
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

    inline auto Matrix::operator-(const Matrix& m) const -> Matrix
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

    inline auto Matrix::operator*(const Matrix& m) const -> Matrix
    {
        auto result(*this);
        return result *= m;
    }

    inline auto operator*=(Vector3& v, const Matrix& m) -> Vector3&
    {
        v = m.transformDirection(v);
        return v;
    }

    inline auto operator*(const Matrix& m, const Vector3& v) -> Vector3
    {
        return m.transformDirection(v);
    }

    inline auto Matrix::getUpVector() const -> Vector3
    {
        return Vector3(m[4], m[5], m[6]);
    }

    inline auto Matrix::getDownVector() const -> Vector3
    {
        return Vector3(-m[4], -m[5], -m[6]);
    }

    inline auto Matrix::getLeftVector() const -> Vector3
    {
        return Vector3(-m[0], -m[1], -m[2]);
    }

    inline auto Matrix::getRightVector() const -> Vector3
    {
        return Vector3(m[0], m[1], m[2]);
    }

    inline auto Matrix::getForwardVector() const -> Vector3
    {
        return Vector3(-m[8], -m[9], -m[10]);
    }

    inline auto Matrix::getBackVector() const -> Vector3
    {
        return Vector3(m[8], m[9], m[10]);
    }

    inline void Matrix::scaleByScalar(float value)
    {
        scaleByVector(Vector3(value, value, value));
    }
}
