/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloMatrix.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace solo
{
    class BoundingBox;
    class BoundingSphere;
    class Plane;
    class Ray;
    class Quaternion;

    class TransformMatrix final: public Matrix
    {
    public:
        TransformMatrix();
        TransformMatrix(const TransformMatrix &other);
        TransformMatrix(const Matrix &other);

        static auto createLookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up) -> TransformMatrix;
        static auto createLookAt2(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up) -> TransformMatrix;
        static auto createPerspective(const Radian &fieldOfView, float aspectRatio, float znear, float zfar) -> TransformMatrix;
        static auto createPerspective2(float fieldOfView, float aspectRatio, float znear, float zfar) -> TransformMatrix;
        static auto createOrthographic(float width, float height, float near, float far) -> TransformMatrix;
        static auto createScale(const Vector3 &scale) -> TransformMatrix;
        static auto createScale2(const glm::vec3 &scale) -> TransformMatrix;
        static auto createRotationFromQuaternion(const Quaternion &quat) -> TransformMatrix;
        static auto createRotationFromQuaternion2(const glm::quat &quat) -> TransformMatrix;
        static auto createRotationFromAxisAngle(const Vector3 &axis, const Radian &angle) -> TransformMatrix;
        static auto createRotationFromAxisAngle2(const glm::vec3 &axis, float angle) -> TransformMatrix;
        static auto createRotationX(const Radian &angle) -> TransformMatrix;
        static auto createRotationX2(float angle) -> TransformMatrix;
        static auto createRotationY(const Radian &angle) -> TransformMatrix;
        static auto createRotationY2(float angle) -> TransformMatrix;
        static auto createRotationZ(const Radian &angle) -> TransformMatrix;
        static auto createRotationZ2(float angle) -> TransformMatrix;
        static auto createTranslation(const Vector3 &translation) -> TransformMatrix;
        static auto createTranslation2(const glm::vec3 &translation) -> TransformMatrix;

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
        auto transformBoundingBox(const BoundingBox &box) -> BoundingBox;
        auto transformBoundingSphere(const BoundingSphere &sphere) -> BoundingSphere;
        auto transformPlane(const Plane &plane) -> Plane;
        auto transformRay(const Ray &ray) -> Ray;

        bool decompose(Vector3 *scale, Quaternion *rotation, Vector3 *translation) const;
        bool decompose2(glm::vec3 *scale, glm::quat *rotation, glm::vec3 *translation) const;
    };

    inline auto TransformMatrix::getUpVector() const -> Vector3
    {
        return {m[4], m[5], m[6]};
    }

    inline auto TransformMatrix::getUpVector2() const -> glm::vec3
    {
        return {m[4], m[5], m[6]};
    }

    inline auto TransformMatrix::getDownVector() const -> Vector3
    {
        return {-m[4], -m[5], -m[6]};
    }

    inline auto TransformMatrix::getDownVector2() const -> glm::vec3
    {
        return {-m[4], -m[5], -m[6]};
    }

    inline auto TransformMatrix::getLeftVector() const -> Vector3
    {
        return {-m[0], -m[1], -m[2]};
    }

    inline auto TransformMatrix::getLeftVector2() const -> glm::vec3
    {
        return {-m[0], -m[1], -m[2]};
    }

    inline auto TransformMatrix::getRightVector() const -> Vector3
    {
        return {m[0], m[1], m[2]};
    }

    inline auto TransformMatrix::getRightVector2() const -> glm::vec3
    {
        return {m[0], m[1], m[2]};
    }

    inline auto TransformMatrix::getForwardVector() const -> Vector3
    {
        return {-m[8], -m[9], -m[10]};
    }

    inline auto TransformMatrix::getForwardVector2() const -> glm::vec3
    {
        return {-m[8], -m[9], -m[10]};
    }

    inline auto TransformMatrix::getBackVector() const -> Vector3
    {
        return {m[8], m[9], m[10]};
    }

    inline auto TransformMatrix::getBackVector2() const -> glm::vec3
    {
        return {m[8], m[9], m[10]};
    }

    inline void TransformMatrix::scaleByScalar(float value)
    {
        scaleByVector({value, value, value});
    }

    inline void TransformMatrix::rotateByQuaternion(const Quaternion &q)
    {
        *this *= createRotationFromQuaternion(q);
    }

    inline void TransformMatrix::rotateByQuaternion2(const glm::quat &q)
    {
        *this *= createRotationFromQuaternion2(q);
    }

    inline void TransformMatrix::rotateByAxisAngle(const Vector3 &axis, const Radian &angle)
    {
        *this *= createRotationFromAxisAngle(axis, angle);
    }

    inline void TransformMatrix::rotateByAxisAngle2(const glm::vec3 &axis, float angle)
    {
        *this *= createRotationFromAxisAngle2(axis, angle);
    }

    inline void TransformMatrix::rotateX(const Radian &angle)
    {
        *this *= createRotationX(angle);
    }

    inline void TransformMatrix::rotateX2(float angle)
    {
        *this *= createRotationX2(angle);
    }

    inline void TransformMatrix::rotateY(const Radian &angle)
    {
        *this *= createRotationY(angle);
    }

    inline void TransformMatrix::rotateY2(float angle)
    {
        *this *= createRotationY2(angle);
    }

    inline void TransformMatrix::rotateZ(const Radian &angle)
    {
        *this *= createRotationZ(angle);
    }

    inline void TransformMatrix::rotateZ2(float angle)
    {
        *this *= createRotationZ2(angle);
    }

    inline void TransformMatrix::scaleByVector(const Vector3 &s)
    {
        *this *= createScale(s);
    }

    inline void TransformMatrix::scaleByVector2(const glm::vec3 &s)
    {
        *this *= createScale2(s);
    }

    inline void TransformMatrix::translate(const Vector3 &t)
    {
        *this *= createTranslation(t);
    }

    inline void TransformMatrix::translate2(const glm::vec3 &t)
    {
        *this *= createTranslation2(t);
    }
}