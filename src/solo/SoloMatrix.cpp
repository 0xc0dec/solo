/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloMatrix.h"
#include "SoloMath.h"
#include "SoloRay.h"
#include "SoloQuaternion.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

using namespace solo;

Matrix::Matrix()
{
    *this = identity();
}

Matrix::Matrix(const glm::mat4x4 &data):
    data_(data)
{
}

Matrix::Matrix(
    float m11, float m12, float m13, float m14,
    float m21, float m22, float m23, float m24,
    float m31, float m32, float m33, float m34,
    float m41, float m42, float m43, float m44)
{
    data_[0][0] = m11;
    data_[0][1] = m21;
    data_[0][2] = m31;
    data_[0][3] = m41;
    data_[1][0] = m12;
    data_[1][1] = m22;
    data_[1][2] = m32;
    data_[1][3] = m42;
    data_[2][0] = m13;
    data_[2][1] = m23;
    data_[2][2] = m33;
    data_[2][3] = m43;
    data_[3][0] = m14;
    data_[3][1] = m24;
    data_[3][2] = m34;
    data_[3][3] = m44;
}

Matrix::Matrix(const Matrix &copy)
{
    data_ = copy.data_;
}

auto Matrix::identity() -> Matrix
{
    static Matrix m(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
    return m;
}

auto Matrix::getDeterminant() const -> float
{
    return glm::determinant(data_);
}

void Matrix::invert()
{
    data_ = glm::inverse(data_);
}

bool Matrix::isIdentity() const
{
    return
        glm::all(glm::epsilonEqual(data_[0], {1.0f, 0.0f, 0.0f, 0.0}, glm::epsilon<float>())) &&
        glm::all(glm::epsilonEqual(data_[1], {0.0f, 1.0f, 0.0f, 0.0}, glm::epsilon<float>())) &&
        glm::all(glm::epsilonEqual(data_[2], {0.0f, 0.0f, 1.0f, 0.0}, glm::epsilon<float>())) &&
        glm::all(glm::epsilonEqual(data_[3], {0.0f, 0.0f, 0.0f, 1.0}, glm::epsilon<float>()));
}

auto Matrix::createLookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up) -> Matrix
{
    // TODO via glm
    const auto zaxis = (eye - target).normalized();
    const auto xaxis = up.normalized().cross(zaxis).normalized();
    const auto yaxis = zaxis.cross(xaxis).normalized();

    // Matrix is built already transposed
    return Matrix(
        xaxis.x(), yaxis.x(), zaxis.x(), 0,
        xaxis.y(), yaxis.y(), zaxis.y(), 0,
        xaxis.z(), yaxis.z(), zaxis.z(), 0,
        -xaxis.dot(eye), -yaxis.dot(eye), -zaxis.dot(eye), 1
    );
}

auto Matrix::createPerspective(const Radians &fieldOfView, float aspectRatio, float znear, float zfar) -> Matrix
{
    return glm::perspective(fieldOfView.toRawRadians(), aspectRatio, znear, zfar);
}

auto Matrix::createOrthographic(float width, float height, float near, float far) -> Matrix
{
    const auto halfWidth = width / 2.0f;
    const auto halfHeight = height / 2.0f;
    const auto left = -halfWidth;
    const auto right = halfWidth;
    const auto top = halfHeight;
    const auto bottom = -halfHeight;
    return glm::ortho(left, right, bottom, top, near, far);
}

auto Matrix::createScale(const Vector3 &scale) -> Matrix
{
    Matrix result;
    result.data_[0][0] = scale.x();
    result.data_[1][1] = scale.y();
    result.data_[2][2] = scale.z();
    return result;
}

auto Matrix::createRotationFromQuaternion(const Quaternion &q) -> Matrix
{
    return glm::mat4_cast(static_cast<glm::quat>(q));
}

auto Matrix::createRotationFromAxisAngle(const Vector3 &axis, const Radians &angle) -> Matrix
{
    return glm::mat4_cast(static_cast<glm::quat>(Quaternion::fromAxisAngle(axis, angle)));
}

auto Matrix::createTranslation(const Vector3 &translation) -> Matrix
{
    Matrix result;
    result.data_[3][0] = translation.x();
    result.data_[3][1] = translation.y();
    result.data_[3][2] = translation.z();
    return result;
}

auto Matrix::columns() const -> const float*
{
    return glm::value_ptr(data_);
}

auto Matrix::scale() const -> Vector3
{
    Quaternion rotation;
    Vector3 scale, translation;
    decompose(scale, rotation, translation);
    return scale;
}

auto Matrix::rotation() const -> Quaternion
{
    Quaternion rotation;
    Vector3 scale, translation;
    decompose(scale, rotation, translation);
    return rotation;
}

auto Matrix::translation() const -> Vector3
{
    return glm::vec3(data_[3]);
}

auto Matrix::transformPoint(const Vector3 &point) const -> Vector3
{
    return static_cast<glm::vec3>(data_ * glm::vec4(static_cast<glm::vec3>(point), 1.0f));
}

auto Matrix::transformDirection(const Vector3 &dir) const -> Vector3
{
    return static_cast<glm::vec3>(data_ * glm::vec4(static_cast<glm::vec3>(dir), 0.0f));
}

auto Matrix::transformRay(const Ray &ray) const -> Ray
{
    const auto origin = transformPoint(ray.origin());
    const auto direction = transformDirection(ray.direction()).normalized();
    return {origin, direction};
}

void Matrix::decompose(Vector3 &scale, Quaternion &rotation, Vector3 &translation) const
{
    glm::vec3 rawScale;
    glm::quat rawRotation;
    glm::vec3 rawTranslation;
    glm::vec3 rawSkew;
    glm::vec4 rawPerspective;
    
    glm::decompose(data_, rawScale, rawRotation, rawTranslation, rawSkew, rawPerspective);

    scale = rawScale;
    rotation = glm::conjugate(rawRotation);
    translation = rawTranslation;
}

auto Matrix::operator+=(float scalar) -> Matrix &
{
    data_ += scalar;
    return *this;
}

auto Matrix::operator+=(const Matrix &other) -> Matrix &
{
    data_ += other.data_;
    return *this;
}

void Matrix::transpose()
{
    data_ = glm::transpose(data_);
}

auto Matrix::operator*=(float scalar) -> Matrix &
{
    data_ *= scalar;
    return *this;
}

auto Matrix::operator*=(const Matrix &m2) -> Matrix &
{
    data_ *= m2.data_;
    return *this;
}

auto Matrix::operator-=(float scalar) -> Matrix &
{
    data_ -= scalar;
    return *this;
}

auto Matrix::operator-=(const Matrix &m2) -> Matrix &
{
    data_ -= m2.data_;
    return *this;
}

auto Matrix::operator-() const -> Matrix
{
    return -data_;
}

void Matrix::scaleByScalar(float value)
{
    scaleByVector({value, value, value});
}

void Matrix::rotateByQuaternion(const Quaternion &q)
{
    *this *= createRotationFromQuaternion(q);
}

void Matrix::rotateByAxisAngle(const Vector3 &axis, const Radians &angle)
{
    *this *= createRotationFromAxisAngle(axis, angle);
}

void Matrix::scaleByVector(const Vector3 &s)
{
    data_ = glm::scale(data_, static_cast<glm::vec3>(s));
}

void Matrix::translate(const Vector3 &t)
{
    data_ = glm::translate(data_, static_cast<glm::vec3>(t));
}

auto Matrix::operator+(float scalar) const -> Matrix
{
    return data_ + scalar;
}

auto Matrix::operator+(const Matrix &m) const -> Matrix
{
    return data_ + m.data_;
}

auto Matrix::operator-(float scalar) const -> Matrix
{
    return data_ - scalar;
}

auto Matrix::operator-(const Matrix &m) const -> Matrix
{
    return data_ - m.data_;
}

auto Matrix::operator*(float scalar) const -> Matrix
{
    return data_ * scalar;
}

auto Matrix::operator*(const Matrix &m) const -> Matrix
{
    return data_ * m.data_;
}
