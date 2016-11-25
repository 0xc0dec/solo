/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloMatrix.h"
#include "SoloMath.h"
#include "SoloCommon.h"

using namespace solo;


static const float IdentityMatrix[16] =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};


Matrix::Matrix()
{
    *this = identity();
}


Matrix::Matrix(
    float m11, float m12, float m13, float m14,
    float m21, float m22, float m23, float m24,
    float m31, float m32, float m33, float m34,
    float m41, float m42, float m43, float m44)
{
    m[0] = m11;
    m[1] = m21;
    m[2] = m31;
    m[3] = m41;
    m[4] = m12;
    m[5] = m22;
    m[6] = m32;
    m[7] = m42;
    m[8] = m13;
    m[9] = m23;
    m[10] = m33;
    m[11] = m43;
    m[12] = m14;
    m[13] = m24;
    m[14] = m34;
    m[15] = m44;
}


Matrix::Matrix(const Matrix &copy)
{
    memcpy(m, copy.m, size());
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


auto Matrix::zero() -> Matrix
{
    static Matrix m(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0);
    return m;
}


auto Matrix::getDeterminant() const -> float
{
    auto a0 = m[0] * m[5] - m[1] * m[4];
    auto a1 = m[0] * m[6] - m[2] * m[4];
    auto a2 = m[0] * m[7] - m[3] * m[4];
    auto a3 = m[1] * m[6] - m[2] * m[5];
    auto a4 = m[1] * m[7] - m[3] * m[5];
    auto a5 = m[2] * m[7] - m[3] * m[6];
    auto b0 = m[8] * m[13] - m[9] * m[12];
    auto b1 = m[8] * m[14] - m[10] * m[12];
    auto b2 = m[8] * m[15] - m[11] * m[12];
    auto b3 = m[9] * m[14] - m[10] * m[13];
    auto b4 = m[9] * m[15] - m[11] * m[13];
    auto b5 = m[10] * m[15] - m[11] * m[14];

    return a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
}


bool Matrix::invert()
{
    auto a0 = m[0] * m[5] - m[1] * m[4];
    auto a1 = m[0] * m[6] - m[2] * m[4];
    auto a2 = m[0] * m[7] - m[3] * m[4];
    auto a3 = m[1] * m[6] - m[2] * m[5];
    auto a4 = m[1] * m[7] - m[3] * m[5];
    auto a5 = m[2] * m[7] - m[3] * m[6];
    auto b0 = m[8] * m[13] - m[9] * m[12];
    auto b1 = m[8] * m[14] - m[10] * m[12];
    auto b2 = m[8] * m[15] - m[11] * m[12];
    auto b3 = m[9] * m[14] - m[10] * m[13];
    auto b4 = m[9] * m[15] - m[11] * m[13];
    auto b5 = m[10] * m[15] - m[11] * m[14];

    auto det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

    // Close to zero, can't invert
    if (math::approxZero(det, math::epsilon3))
        return false;

    // Support the case where m == dst
    Matrix inverse;
    inverse.m[0] = m[5] * b5 - m[6] * b4 + m[7] * b3;
    inverse.m[1] = -m[1] * b5 + m[2] * b4 - m[3] * b3;
    inverse.m[2] = m[13] * a5 - m[14] * a4 + m[15] * a3;
    inverse.m[3] = -m[9] * a5 + m[10] * a4 - m[11] * a3;

    inverse.m[4] = -m[4] * b5 + m[6] * b2 - m[7] * b1;
    inverse.m[5] = m[0] * b5 - m[2] * b2 + m[3] * b1;
    inverse.m[6] = -m[12] * a5 + m[14] * a2 - m[15] * a1;
    inverse.m[7] = m[8] * a5 - m[10] * a2 + m[11] * a1;

    inverse.m[8] = m[4] * b4 - m[5] * b2 + m[7] * b0;
    inverse.m[9] = -m[0] * b4 + m[1] * b2 - m[3] * b0;
    inverse.m[10] = m[12] * a4 - m[13] * a2 + m[15] * a0;
    inverse.m[11] = -m[8] * a4 + m[9] * a2 - m[11] * a0;

    inverse.m[12] = -m[4] * b3 + m[5] * b1 - m[6] * b0;
    inverse.m[13] = m[0] * b3 - m[1] * b1 + m[2] * b0;
    inverse.m[14] = -m[12] * a3 + m[13] * a1 - m[14] * a0;
    inverse.m[15] = m[8] * a3 - m[9] * a1 + m[10] * a0;

    *this = inverse * (1.0f / det);

    return true;
}


bool Matrix::isIdentity() const
{
    return memcmp(m, IdentityMatrix, size()) == 0;
}


void Matrix::makeIdentity()
{
    memcpy(m, IdentityMatrix, size());
}


void Matrix::makeZero()
{
    memset(m, 0, size());
}


auto Matrix::operator+=(float scalar) -> Matrix &

{
    m[0] += scalar;
    m[1] += scalar;
    m[2] += scalar;
    m[3] += scalar;
    m[4] += scalar;
    m[5] += scalar;
    m[6] += scalar;
    m[7] += scalar;
    m[8] += scalar;
    m[9] += scalar;
    m[10] += scalar;
    m[11] += scalar;
    m[12] += scalar;
    m[13] += scalar;
    m[14] += scalar;
    m[15] += scalar;
    return *this;
}


auto Matrix::operator+=(const Matrix &other) -> Matrix &

{
    m[0] += other.m[0];
    m[1] += other.m[1];
    m[2] += other.m[2];
    m[3] += other.m[3];
    m[4] += other.m[4];
    m[5] += other.m[5];
    m[6] += other.m[6];
    m[7] += other.m[7];
    m[8] += other.m[8];
    m[9] += other.m[9];
    m[10] += other.m[10];
    m[11] += other.m[11];
    m[12] += other.m[12];
    m[13] += other.m[13];
    m[14] += other.m[14];
    m[15] += other.m[15];
    return *this;
}


void Matrix::transpose()
{
    float t[16] =
    {
        m[0], m[4], m[8], m[12],
        m[1], m[5], m[9], m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    };
    memcpy(&m, t, size());
}


auto Matrix::operator*=(float scalar) -> Matrix &

{
    m[0] *= scalar;
    m[1] *= scalar;
    m[2] *= scalar;
    m[3] *= scalar;
    m[4] *= scalar;
    m[5] *= scalar;
    m[6] *= scalar;
    m[7] *= scalar;
    m[8] *= scalar;
    m[9] *= scalar;
    m[10] *= scalar;
    m[11] *= scalar;
    m[12] *= scalar;
    m[13] *= scalar;
    m[14] *= scalar;
    m[15] *= scalar;
    return *this;
}


auto Matrix::operator*=(const Matrix &m2) -> Matrix &

{
    float product[16];

    product[0] = m[0] * m2.m[0] + m[4] * m2.m[1] + m[8] * m2.m[2] + m[12] * m2.m[3];
    product[1] = m[1] * m2.m[0] + m[5] * m2.m[1] + m[9] * m2.m[2] + m[13] * m2.m[3];
    product[2] = m[2] * m2.m[0] + m[6] * m2.m[1] + m[10] * m2.m[2] + m[14] * m2.m[3];
    product[3] = m[3] * m2.m[0] + m[7] * m2.m[1] + m[11] * m2.m[2] + m[15] * m2.m[3];

    product[4] = m[0] * m2.m[4] + m[4] * m2.m[5] + m[8] * m2.m[6] + m[12] * m2.m[7];
    product[5] = m[1] * m2.m[4] + m[5] * m2.m[5] + m[9] * m2.m[6] + m[13] * m2.m[7];
    product[6] = m[2] * m2.m[4] + m[6] * m2.m[5] + m[10] * m2.m[6] + m[14] * m2.m[7];
    product[7] = m[3] * m2.m[4] + m[7] * m2.m[5] + m[11] * m2.m[6] + m[15] * m2.m[7];

    product[8] = m[0] * m2.m[8] + m[4] * m2.m[9] + m[8] * m2.m[10] + m[12] * m2.m[11];
    product[9] = m[1] * m2.m[8] + m[5] * m2.m[9] + m[9] * m2.m[10] + m[13] * m2.m[11];
    product[10] = m[2] * m2.m[8] + m[6] * m2.m[9] + m[10] * m2.m[10] + m[14] * m2.m[11];
    product[11] = m[3] * m2.m[8] + m[7] * m2.m[9] + m[11] * m2.m[10] + m[15] * m2.m[11];

    product[12] = m[0] * m2.m[12] + m[4] * m2.m[13] + m[8] * m2.m[14] + m[12] * m2.m[15];
    product[13] = m[1] * m2.m[12] + m[5] * m2.m[13] + m[9] * m2.m[14] + m[13] * m2.m[15];
    product[14] = m[2] * m2.m[12] + m[6] * m2.m[13] + m[10] * m2.m[14] + m[14] * m2.m[15];
    product[15] = m[3] * m2.m[12] + m[7] * m2.m[13] + m[11] * m2.m[14] + m[15] * m2.m[15];

    memcpy(m, product, size());

    return *this;
}


auto Matrix::operator-=(float scalar) -> Matrix &

{
    m[0] -= scalar;
    m[1] -= scalar;
    m[2] -= scalar;
    m[3] -= scalar;
    m[4] -= scalar;
    m[5] -= scalar;
    m[6] -= scalar;
    m[7] -= scalar;
    m[8] -= scalar;
    m[9] -= scalar;
    m[10] -= scalar;
    m[11] -= scalar;
    m[12] -= scalar;
    m[13] -= scalar;
    m[14] -= scalar;
    m[15] -= scalar;
    return *this;
}


auto Matrix::operator-=(const Matrix &m2) -> Matrix &

{
    m[0] -= m2.m[0];
    m[1] -= m2.m[1];
    m[2] -= m2.m[2];
    m[3] -= m2.m[3];
    m[4] -= m2.m[4];
    m[5] -= m2.m[5];
    m[6] -= m2.m[6];
    m[7] -= m2.m[7];
    m[8] -= m2.m[8];
    m[9] -= m2.m[9];
    m[10] -= m2.m[10];
    m[11] -= m2.m[11];
    m[12] -= m2.m[12];
    m[13] -= m2.m[13];
    m[14] -= m2.m[14];
    m[15] -= m2.m[15];
    return *this;
}


auto Matrix::operator-() const -> Matrix
{
    Matrix result;
    result.m[0] = -m[0];
    result.m[1] = -m[1];
    result.m[2] = -m[2];
    result.m[3] = -m[3];
    result.m[4] = -m[4];
    result.m[5] = -m[5];
    result.m[6] = -m[6];
    result.m[7] = -m[7];
    result.m[8] = -m[8];
    result.m[9] = -m[9];
    result.m[10] = -m[10];
    result.m[11] = -m[11];
    result.m[12] = -m[12];
    result.m[13] = -m[13];
    result.m[14] = -m[14];
    result.m[15] = -m[15];
    return result;
}

