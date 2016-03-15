#pragma once

#include "SoloVector3.h"
#include "SoloVector4.h"


namespace solo
{
    struct Plane;
    struct Quaternion;

    struct Matrix final
    {
        // column-major order
        float m[16];

        Matrix();
        Matrix(float m11, float m12, float m13, float m14,
               float m21, float m22, float m23, float m24,
               float m31, float m32, float m33, float m34,
               float m41, float m42, float m43, float m44);
        Matrix(const Matrix& copy);

        static Matrix identity();
        static Matrix zero();

        bool isIdentity() const;

        static Matrix createLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up);

        static Matrix createPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane);
        static Matrix createOrthographic(float width, float height, float zNearPlane, float zFarPlane);
        static Matrix createOrthographicOffCenter(float left, float right, float bottom, float top, float near, float far);

        static Matrix createReflection(const Plane& plane);
        static Matrix createScale(const Vector3& scale);
        static Matrix createRotation(const Quaternion& quat);
        static Matrix createRotation(const Vector3& axis, float angleRadians);
        static Matrix createRotationX(float angleRadians);
        static Matrix createRotationY(float angleRadians);
        static Matrix createRotationZ(float angleRadians);
        static Matrix createTranslation(const Vector3& translation);

        bool decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const;

        float getDeterminant() const;

        Vector3 getScale() const;
        Quaternion getRotation() const;
        Vector3 getTranslation() const;

        Vector3 getUpVector() const;
        Vector3 getDownVector() const;
        Vector3 getLeftVector() const;
        Vector3 getRightVector() const;
        Vector3 getForwardVector() const;
        Vector3 getBackVector() const;

        bool invert();
        void transpose();

        void rotate(const Quaternion& q);
        void rotate(const Vector3& axis, float angleRadians);
        void rotateX(float angleRadians);
        void rotateY(float angleRadians);
        void rotateZ(float angleRadians);

        void scale(float value);
        void scale(const Vector3& s);

        void translate(const Vector3& t);

        void setIdentity();
        void setZero();

        inline Matrix operator+(float scalar) const;
        inline Matrix operator+(const Matrix& m) const;
        Matrix& operator+=(float scalar);
        Matrix& operator+=(const Matrix& m);

        Matrix operator-() const;
        inline Matrix operator-(float scalar) const;
        inline Matrix operator-(const Matrix& m) const;
        Matrix& operator-=(float scalar);
        Matrix& operator-=(const Matrix& m);

        inline Matrix operator*(float scalar) const;
        inline Matrix operator*(const Matrix& m) const;
        Matrix& operator*=(float scalar);
        Matrix& operator*=(const Matrix& m);

        Vector3 transformPoint(const Vector3& point) const;

        Vector3 transformDirection(const Vector3& dir) const;
        Vector4 transformDirection(const Vector4& dir) const;
    };

    inline Matrix Matrix::operator+(float scalar) const
    {
        auto result(*this);
        result += scalar;
        return result;
    }

    inline Matrix Matrix::operator+(const Matrix& m) const
    {
        auto result(*this);
        result += m;
        return result;
    }

    inline Matrix Matrix::operator-(float scalar) const
    {
        auto result(*this);
        result -= scalar;
        return result;
    }

    inline Matrix Matrix::operator-(const Matrix& m) const
    {
        auto result(*this);
        result -= m;
        return result;
    }

    Matrix Matrix::operator*(float scalar) const
    {
        auto result(*this);
        result *= scalar;
        return result;
    }

    inline Matrix Matrix::operator*(const Matrix& m) const
    {
        auto result(*this);
        return result *= m;
    }

    inline Vector3& operator*=(Vector3& v, const Matrix& m)
    {
        v = m.transformDirection(v);
        return v;
    }

    inline Vector3 operator*(const Matrix& m, const Vector3& v)
    {
        return m.transformDirection(v);
    }

    inline Vector4& operator*=(Vector4& v, const Matrix& m)
    {
        v = m.transformDirection(v);
        return v;
    }

    inline Vector4 operator*(const Matrix& m, const Vector4& v)
    {
        return m.transformDirection(v);
    }

    inline Vector3 Matrix::getUpVector() const
    {
        return Vector3(m[4], m[5], m[6]);
    }

    inline Vector3 Matrix::getDownVector() const
    {
        return Vector3(-m[4], -m[5], -m[6]);
    }

    inline Vector3 Matrix::getLeftVector() const
    {
        return Vector3(-m[0], -m[1], -m[2]);
    }

    inline Vector3 Matrix::getRightVector() const
    {
        return Vector3(m[0], m[1], m[2]);
    }

    inline Vector3 Matrix::getForwardVector() const
    {
        return Vector3(-m[8], -m[9], -m[10]);
    }

    inline Vector3 Matrix::getBackVector() const
    {
        return Vector3(m[8], m[9], m[10]);
    }

    inline void Matrix::scale(float value)
    {
        scale(Vector3(value, value, value));
    }
}
