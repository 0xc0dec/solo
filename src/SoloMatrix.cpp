#include "SoloMatrix.h"
#include "SoloMath.h"
#include "SoloQuaternion.h"
#include "SoloPlane.h"
#include "SoloBase.h"
#include "SoloRadian.h"

using namespace solo;


static const float IdentityMatrix[16] =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};


const size_t MatrixSize = sizeof(float) * 16;


Matrix::Matrix()
{
    *this = identity();
}


Matrix::Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
    float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
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


Matrix::Matrix(const Matrix& copy)
{
    memcpy(m, copy.m, MatrixSize);
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


auto Matrix::createLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) -> Matrix
{
    auto zaxis(eye - target);
    zaxis.normalize();

    auto xaxis = Vector3::cross(up.normalized(), zaxis);
    xaxis.normalize();

    auto yaxis = Vector3::cross(zaxis, xaxis);
    yaxis.normalize();

    // Matrix is built already transposed
    return Matrix(
        xaxis.x, yaxis.x, zaxis.x, 0,
        xaxis.y, yaxis.y, zaxis.y, 0,
        xaxis.z, yaxis.z, zaxis.z, 0,
        -Vector3::dot(xaxis, eye), -Vector3::dot(yaxis, eye), -Vector3::dot(zaxis, eye), 1
    );
}


auto Matrix::createPerspective(const Radian& fieldOfView, float aspectRatio, float znear, float zfar) -> Matrix
{
    auto f_n = 1.0f / (zfar - znear);
    auto theta = fieldOfView.toRawRadian() * 0.5f;
    // TODO asserts instead?
    SL_DEBUG_FMT_THROW_IF(Math::approxZero(fmod(theta, Math::piOver2), Math::smallFloat1),
        InvalidInputException, "Invalid field of view value ", fieldOfView.toRawDegree(), " caused attempted tan calculation, which is undefined");

    auto divisor = tan(theta);
    auto factor = 1.0f / divisor;

    Matrix result;
    memset(&result.m, 0, MatrixSize);
    result.m[0] = (1.0f / aspectRatio) * factor;
    result.m[5] = factor;
    result.m[10] = -(zfar + znear) * f_n;
    result.m[11] = -1.0f;
    result.m[14] = -2.0f * zfar * znear * f_n;

    return result;
}


auto Matrix::createOrthographic(float width, float height, float near, float far) -> Matrix
{
    auto halfWidth = width / 2.0f;
    auto halfHeight = height / 2.0f;
    auto left = -halfWidth;
    auto right = halfWidth;
    auto top = halfHeight;
    auto bottom = -halfHeight;
    Matrix result;
    memset(&result.m, 0, MatrixSize);
    result.m[0] = 2 / (right - left);
    result.m[5] = 2 / (top - bottom);
    result.m[12] = (left + right) / (left - right);
    result.m[10] = 1 / (near - far);
    result.m[13] = (top + bottom) / (bottom - top);
    result.m[14] = near / (near - far);
    result.m[15] = 1;
    return result;
}


auto Matrix::createReflection(const Plane& plane) -> Matrix
{
    auto normal(plane.getNormal());
    auto k = -2.0f * plane.getDistance();

    Matrix result;

    result.m[0] -= 2.0f * normal.x * normal.x;
    result.m[5] -= 2.0f * normal.y * normal.y;
    result.m[10] -= 2.0f * normal.z * normal.z;
    result.m[1] = result.m[4] = -2.0f * normal.x * normal.y;
    result.m[2] = result.m[8] = -2.0f * normal.x * normal.z;
    result.m[6] = result.m[9] = -2.0f * normal.y * normal.z;

    result.m[3] = k * normal.x;
    result.m[7] = k * normal.y;
    result.m[11] = k * normal.z;

    return result;
}


auto Matrix::createScale(const Vector3& scale) -> Matrix
{
    Matrix result;
    result.m[0] = scale.x;
    result.m[5] = scale.y;
    result.m[10] = scale.z;
    return result;
}


auto Matrix::createRotationFromQuaternion(const Quaternion& q) -> Matrix
{
    auto x2 = q.x + q.x;
    auto y2 = q.y + q.y;
    auto z2 = q.z + q.z;

    auto xx2 = q.x * x2;
    auto yy2 = q.y * y2;
    auto zz2 = q.z * z2;
    auto xy2 = q.x * y2;
    auto xz2 = q.x * z2;
    auto yz2 = q.y * z2;
    auto wx2 = q.w * x2;
    auto wy2 = q.w * y2;
    auto wz2 = q.w * z2;

    Matrix result;

    result.m[0] = 1.0f - yy2 - zz2;
    result.m[1] = xy2 + wz2;
    result.m[2] = xz2 - wy2;
    result.m[3] = 0.0f;

    result.m[4] = xy2 - wz2;
    result.m[5] = 1.0f - xx2 - zz2;
    result.m[6] = yz2 + wx2;
    result.m[7] = 0.0f;

    result.m[8] = xz2 + wy2;
    result.m[9] = yz2 - wx2;
    result.m[10] = 1.0f - xx2 - yy2;
    result.m[11] = 0.0f;

    result.m[12] = 0.0f;
    result.m[13] = 0.0f;
    result.m[14] = 0.0f;
    result.m[15] = 1.0f;

    return result;
}


auto Matrix::createRotationFromAxisAngle(const Vector3& axis, const Radian& angle) -> Matrix
{
    auto x = axis.x;
    auto y = axis.y;
    auto z = axis.z;

    // Make sure the input axis is normalized
    auto n = x * x + y * y + z * z;
    if (n != 1.0f)
    {
        // Not normalized
        n = sqrt(n);
        // Prevent division too close to zero.
        if (!Math::approxZero(n, Math::smallFloat1))
        {
            n = 1.0f / n;
            x *= n;
            y *= n;
            z *= n;
        }
    }

    auto c = cos(angle.toRawRadian());
    auto s = sin(angle.toRawRadian());

    auto t = 1.0f - c;
    auto tx = t * x;
    auto ty = t * y;
    auto tz = t * z;
    auto txy = tx * y;
    auto txz = tx * z;
    auto tyz = ty * z;
    auto sx = s * x;
    auto sy = s * y;
    auto sz = s * z;

    Matrix result;

    result.m[0] = c + tx * x;
    result.m[1] = txy + sz;
    result.m[2] = txz - sy;
    result.m[3] = 0.0f;

    result.m[4] = txy - sz;
    result.m[5] = c + ty * y;
    result.m[6] = tyz + sx;
    result.m[7] = 0.0f;

    result.m[8] = txz + sy;
    result.m[9] = tyz - sx;
    result.m[10] = c + tz * z;
    result.m[11] = 0.0f;

    result.m[12] = 0.0f;
    result.m[13] = 0.0f;
    result.m[14] = 0.0f;
    result.m[15] = 1.0f;

    return result;
}


auto Matrix::createRotationX(const Radian& angle) -> Matrix
{
    auto c = cos(angle.toRawRadian());
    auto s = sin(angle.toRawRadian());

    Matrix result;
    result.m[5] = c;
    result.m[6] = s;
    result.m[9] = -s;
    result.m[10] = c;
    return result;
}


auto Matrix::createRotationY(const Radian& angle) -> Matrix
{
    auto c = cos(angle.toRawRadian());
    auto s = sin(angle.toRawRadian());

    Matrix result;

    result.m[0] = c;
    result.m[2] = -s;
    result.m[8] = s;
    result.m[10] = c;

    return result;
}


auto Matrix::createRotationZ(const Radian& angle) -> Matrix
{
    auto c = cos(angle.toRawRadian());
    auto s = sin(angle.toRawRadian());

    Matrix result;
    result.m[0] = c;
    result.m[1] = s;
    result.m[4] = -s;
    result.m[5] = c;
    return result;
}


auto Matrix::createTranslation(const Vector3& translation) -> Matrix
{
    Matrix result;
    result.m[12] = translation.x;
    result.m[13] = translation.y;
    result.m[14] = translation.z;
    return result;
}


bool Matrix::decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const
{
    if (translation)
    {
        translation->x = m[12];
        translation->y = m[13];
        translation->z = m[14];
    }

    if (scale == nullptr && rotation == nullptr)
        return true;

    // Extract the scale.
    // This is simply the length of each axis (row/column) in the matrix.
    Vector3 xaxis(m[0], m[1], m[2]);
    auto scaleX = xaxis.length();

    Vector3 yaxis(m[4], m[5], m[6]);
    auto scaleY = yaxis.length();

    Vector3 zaxis(m[8], m[9], m[10]);
    auto scaleZ = zaxis.length();

    // Determine if we have a negative scale (true if determinant is less than zero).
    // In this case, we simply negate a single axis of the scale.
    auto det = getDeterminant();
    if (det < 0)
        scaleZ = -scaleZ;

    if (scale)
    {
        scale->x = scaleX;
        scale->y = scaleY;
        scale->z = scaleZ;
    }

    if (rotation == nullptr)
        return true;

    // Scale too close to zero, can't decompose rotation.
    if (scaleX < Math::smallFloat3 || scaleY < Math::smallFloat3 || scaleZ < Math::smallFloat3)
        return false;

    float rn;

    // Factor the scale out of the matrix axes.
    rn = 1.0f / scaleX;
    xaxis.x *= rn;
    xaxis.y *= rn;
    xaxis.z *= rn;

    rn = 1.0f / scaleY;
    yaxis.x *= rn;
    yaxis.y *= rn;
    yaxis.z *= rn;

    rn = 1.0f / scaleZ;
    zaxis.x *= rn;
    zaxis.y *= rn;
    zaxis.z *= rn;

    // Calculate the rotation from the resulting matrix (axes).
    auto trace = xaxis.x + yaxis.y + zaxis.z + 1.0f;

    if (trace > Math::smallFloat1)
    {
        auto s = 0.5f / sqrt(trace);
        rotation->w = 0.25f / s;
        rotation->x = (yaxis.z - zaxis.y) * s;
        rotation->y = (zaxis.x - xaxis.z) * s;
        rotation->z = (xaxis.y - yaxis.x) * s;
    }
    else
    {
        // Note: since xaxis, yaxis, and zaxis are normalized,
        // we will never divide by zero in the code below.
        if (xaxis.x > yaxis.y && xaxis.x > zaxis.z)
        {
            auto s = 0.5f / sqrt(1.0f + xaxis.x - yaxis.y - zaxis.z);
            rotation->w = (yaxis.z - zaxis.y) * s;
            rotation->x = 0.25f / s;
            rotation->y = (yaxis.x + xaxis.y) * s;
            rotation->z = (zaxis.x + xaxis.z) * s;
        }
        else if (yaxis.y > zaxis.z)
        {
            auto s = 0.5f / sqrt(1.0f + yaxis.y - xaxis.x - zaxis.z);
            rotation->w = (zaxis.x - xaxis.z) * s;
            rotation->x = (yaxis.x + xaxis.y) * s;
            rotation->y = 0.25f / s;
            rotation->z = (zaxis.y + yaxis.z) * s;
        }
        else
        {
            auto s = 0.5f / sqrt(1.0f + zaxis.z - xaxis.x - yaxis.y);
            rotation->w = (xaxis.y - yaxis.x) * s;
            rotation->x = (zaxis.x + xaxis.z) * s;
            rotation->y = (zaxis.y + yaxis.z) * s;
            rotation->z = 0.25f / s;
        }
    }

    return true;
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


auto Matrix::getScale() const -> Vector3
{
    Vector3 result;
    decompose(&result, nullptr, nullptr);
    return result;
}


auto Matrix::getRotation() const -> Quaternion
{
    Quaternion result;
    decompose(nullptr, &result, nullptr);
    return result;
}


auto Matrix::getTranslation() const -> Vector3
{
    Vector3 result;
    decompose(nullptr, nullptr, &result);
    return result;
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
    if (Math::approxZero(det, Math::smallFloat3))
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
    return memcmp(m, IdentityMatrix, MatrixSize) == 0;
}


void Matrix::rotateByQuaternion(const Quaternion& q)
{
    auto r = createRotationFromQuaternion(q);
    *this *= r;
}


void Matrix::rotateByAxisAngle(const Vector3& axis, const Radian& angle)
{
    auto r = createRotationFromAxisAngle(axis, angle);
    *this *= r;
}


void Matrix::rotateX(const Radian& angle)
{
    auto r = createRotationX(angle);
    *this *= r;
}


void Matrix::rotateY(const Radian& angle)
{
    auto r = createRotationY(angle);
    *this *= r;
}


void Matrix::rotateZ(const Radian& angle)
{
    auto r = createRotationZ(angle);
    *this *= r;
}


void Matrix::scaleByVector(const Vector3& s)
{
    auto sm = createScale(s);
    *this *= sm;
}


void Matrix::setIdentity()
{
    memcpy(m, IdentityMatrix, MatrixSize);
}


void Matrix::setZero()
{
    memset(m, 0, MatrixSize);
}


auto Matrix::operator+=(float scalar) -> Matrix&

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


auto Matrix::operator+=(const Matrix& other) -> Matrix&

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


auto Matrix::transformPoint(const Vector3& point) const -> Vector3
{
    return Vector3(
        point.x * m[0] + point.y * m[4] + point.z * m[8] + m[12],
        point.x * m[1] + point.y * m[5] + point.z * m[9] + m[13],
        point.x * m[2] + point.y * m[6] + point.z * m[10] + m[14]
    );
}


auto Matrix::transformDirection(const Vector3& dir) const -> Vector3
{
    return Vector3(
        dir.x * m[0] + dir.y * m[4] + dir.z * m[8],
        dir.x * m[1] + dir.y * m[5] + dir.z * m[9],
        dir.x * m[2] + dir.y * m[6] + dir.z * m[10]
    );
}


void Matrix::translate(const Vector3& t)
{
    auto tm = createTranslation(t);
    *this *= tm;
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
    memcpy(&m, t, MatrixSize);
}


auto Matrix::operator*=(float scalar) -> Matrix&

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


auto Matrix::operator*=(const Matrix& m2) -> Matrix&

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

    memcpy(m, product, MatrixSize);

    return *this;
}


auto Matrix::operator-=(float scalar) -> Matrix&

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


auto Matrix::operator-=(const Matrix& m2) -> Matrix&

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

