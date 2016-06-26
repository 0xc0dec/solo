#pragma once

#include "SoloMatrix.h"


namespace solo
{
    class Matrix;

    class TransformMatrix
    {
    public:
        Matrix matrix;

        TransformMatrix();
        TransformMatrix(const TransformMatrix& other);
        explicit TransformMatrix(const Matrix& m);

        static auto createLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) -> TransformMatrix;
        static auto createPerspective(const Radian& fieldOfView, float aspectRatio, float znear, float zfar) -> TransformMatrix;
        static auto createOrthographic(float width, float height, float near, float far) -> TransformMatrix;
        static auto createReflection(const Plane& plane) -> TransformMatrix;
        static auto createScale(const Vector3& scale) -> TransformMatrix;
        static auto createRotationFromQuaternion(const Quaternion& quat) -> TransformMatrix;
        static auto createRotationFromAxisAngle(const Vector3& axis, const Radian& angle) -> TransformMatrix;
        static auto createRotationX(const Radian& angle) -> TransformMatrix;
        static auto createRotationY(const Radian& angle) -> TransformMatrix;
        static auto createRotationZ(const Radian& angle) -> TransformMatrix;
        static auto createTranslation(const Vector3& translation) -> TransformMatrix;

        auto getScale() const -> Vector3;
        auto getRotation() const -> Quaternion;
        auto getTranslation() const -> Vector3;

        auto getUpVector() const -> Vector3;
        auto getDownVector() const -> Vector3;
        auto getLeftVector() const -> Vector3;
        auto getRightVector() const -> Vector3;
        auto getForwardVector() const -> Vector3;
        auto getBackVector() const -> Vector3;

        void rotateByQuaternion(const Quaternion& q);
        void rotateByAxisAngle(const Vector3& axis, const Radian& angle);
        void rotateX(const Radian& angle);
        void rotateY(const Radian& angle);
        void rotateZ(const Radian& angle);

        void scaleByScalar(float value);
        void scaleByVector(const Vector3& s);

        void translate(const Vector3& t);

        auto transformPoint(const Vector3& point) const -> Vector3;
        auto transformDirection(const Vector3& dir) const -> Vector3;

        bool decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const;
    };

    inline auto TransformMatrix::getUpVector() const -> Vector3
    {
        return Vector3(matrix.m[4], matrix.m[5], matrix.m[6]);
    }

    inline auto TransformMatrix::getDownVector() const -> Vector3
    {
        return Vector3(-matrix.m[4], -matrix.m[5], -matrix.m[6]);
    }

    inline auto TransformMatrix::getLeftVector() const -> Vector3
    {
        return Vector3(-matrix.m[0], -matrix.m[1], -matrix.m[2]);
    }

    inline auto TransformMatrix::getRightVector() const -> Vector3
    {
        return Vector3(matrix.m[0], matrix.m[1], matrix.m[2]);
    }

    inline auto TransformMatrix::getForwardVector() const -> Vector3
    {
        return Vector3(-matrix.m[8], -matrix.m[9], -matrix.m[10]);
    }

    inline auto TransformMatrix::getBackVector() const -> Vector3
    {
        return Vector3(matrix.m[8], matrix.m[9], matrix.m[10]);
    }

    inline void TransformMatrix::scaleByScalar(float value)
    {
        scaleByVector(Vector3(value, value, value));
    }

    inline void TransformMatrix::rotateByQuaternion(const Quaternion& q)
    {
        matrix *= createRotationFromQuaternion(q).matrix;
    }

    inline void TransformMatrix::rotateByAxisAngle(const Vector3& axis, const Radian& angle)
    {
        matrix *= createRotationFromAxisAngle(axis, angle).matrix;
    }

    inline void TransformMatrix::rotateX(const Radian& angle)
    {
        matrix *= createRotationX(angle).matrix;
    }

    inline void TransformMatrix::rotateY(const Radian& angle)
    {
        matrix *= createRotationY(angle).matrix;
    }

    inline void TransformMatrix::rotateZ(const Radian& angle)
    {
        matrix *= createRotationZ(angle).matrix;
    }
}