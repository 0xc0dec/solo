#include "SoloQuaternion.h"
#include "SoloMath.h"

using namespace solo;


Quaternion::Quaternion(float x, float y, float z, float w):
    x(x), y(y), z(z), w(w)
{
}


Quaternion::Quaternion(const Vector3& axis, float angleRadians)
{
    *this = createFromAxisAngle(axis, angleRadians);
}


Quaternion Quaternion::identity()
{
    static Quaternion value(0.0f, 0.0f, 0.0f, 1.0f);
    return value;
}


Quaternion Quaternion::zero()
{
    static Quaternion value(0.0f, 0.0f, 0.0f, 0.0f);
    return value;
}


bool Quaternion::isIdentity() const
{
    return Math::approxZero(x) && Math::approxZero(y) && Math::approxZero(z) && Math::approxEqual(w, 1.0f);
}


bool Quaternion::isZero() const
{
    return Math::approxZero(x) && Math::approxZero(y) && Math::approxZero(z) && Math::approxZero(w);
}


Quaternion Quaternion::createFromRotationMatrix(const Matrix& m)
{
    return m.getRotation(); // TODO remove
}


Quaternion Quaternion::createFromAxisAngle(const Vector3& axis, float angleRadians)
{
    auto halfAngle = angleRadians * 0.5f;
    auto sinHalfAngle = sinf(halfAngle);
    auto normal(const_cast<Vector3&>(axis));
    normal.normalize();
    return Quaternion(normal.x * sinHalfAngle, normal.y * sinHalfAngle, normal.z * sinHalfAngle, cosf(halfAngle));
}


void Quaternion::conjugate()
{
    x = -x;
    y = -y;
    z = -z;
}


bool Quaternion::inverse()
{
    auto n = x * x + y * y + z * z + w * w;
    if (Math::approxEqual(n, 1.0f))
    {
        x = -x;
        y = -y;
        z = -z;
        return true;
    }

    if (Math::approxZero(n))
        return false;

    n = 1.0f / n;
    x = -x * n;
    y = -y * n;
    z = -z * n;
    w = w * n;

    return true;
}


void Quaternion::normalize()
{
    auto n = x * x + y * y + z * z + w * w;

    // Already normalized
    if (Math::approxEqual(n, 1.0f))
        return;

    n = sqrt(n);
    if (Math::approxZero(n))
        return;

    n = 1.0f / n;
    x *= n;
    y *= n;
    z *= n;
    w *= n;
}


Quaternion Quaternion::normalized() const
{
    auto result(*this);
    result.normalize();
    return result;
}


float Quaternion::toAxisAngle(Vector3& axis) const
{
    Quaternion q(x, y, z, w);
    q.normalize();
    axis.x = q.x;
    axis.y = q.y;
    axis.z = q.z;
    axis.normalize();
    return 2.0f * acos(q.w);
}


Quaternion Quaternion::lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
    if (Math::approxZero(t))
        return q1;

    if (Math::approxEqual(t, 1.0f))
        return q2;

    auto t1 = 1.0f - t;

    return Quaternion(
               t1 * q1.x + t * q2.x,
               t1 * q1.y + t * q2.y,
               t1 * q1.z + t * q2.z,
               t1 * q1.w + t * q2.w
           );
}


Quaternion Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
    if (Math::approxZero(t))
        return q1;

    if (Math::approxEqual(t, 1.0f))
        return q2;

    if (Math::approxEqual(q1.x, q2.x) && Math::approxEqual(q1.y, q2.y) && Math::approxEqual(q1.z, q2.z) && Math::approxEqual(q1.w, q2.w))
        return q1;

    float halfY, alpha, beta;
    float u, f1, f2a, f2b;
    float ratio1, ratio2;
    float halfSecHalfTheta, versHalfTheta;
    float sqNotU, sqU;

    auto cosTheta = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

    alpha = cosTheta >= 0 ? 1.0f : -1.0f;
    halfY = 1.0f + alpha * cosTheta;

    f2b = t - 0.5f;
    u = f2b >= 0 ? f2b : -f2b;
    f2a = u - f2b;
    f2b += u;
    u += u;
    f1 = 1.0f - u;

    halfSecHalfTheta = 1.09f - (0.476537f - 0.0903321f * halfY) * halfY;
    halfSecHalfTheta *= 1.5f - halfY * halfSecHalfTheta * halfSecHalfTheta;
    versHalfTheta = 1.0f - halfY * halfSecHalfTheta;

    sqNotU = f1 * f1;
    ratio2 = 0.0000440917108f * versHalfTheta;
    ratio1 = -0.00158730159f + (sqNotU - 16.0f) * ratio2;
    ratio1 = 0.0333333333f + ratio1 * (sqNotU - 9.0f) * versHalfTheta;
    ratio1 = -0.333333333f + ratio1 * (sqNotU - 4.0f) * versHalfTheta;
    ratio1 = 1.0f + ratio1 * (sqNotU - 1.0f) * versHalfTheta;

    sqU = u * u;
    ratio2 = -0.00158730159f + (sqU - 16.0f) * ratio2;
    ratio2 = 0.0333333333f + ratio2 * (sqU - 9.0f) * versHalfTheta;
    ratio2 = -0.333333333f + ratio2 * (sqU - 4.0f) * versHalfTheta;
    ratio2 = 1.0f + ratio2 * (sqU - 1.0f) * versHalfTheta;

    f1 *= ratio1 * halfSecHalfTheta;
    f2a *= ratio2;
    f2b *= ratio2;
    alpha *= f1 + f2a;
    beta = f1 + f2b;

    auto w = alpha * q1.w + beta * q2.w;
    auto x = alpha * q1.x + beta * q2.x;
    auto y = alpha * q1.y + beta * q2.y;
    auto z = alpha * q1.z + beta * q2.z;

    // Quaternion length correction, if needed
    f1 = 1.5f - 0.5f * (w * w + x * x + y * y + z * z);
    return Quaternion(w * f1, x * f1, y * f1, z * f1);
}


Quaternion Quaternion::squad(const Quaternion& q1, const Quaternion& q2, const Quaternion& s1, const Quaternion& s2, float t)
{
    auto q = slerpForSquad(q1, q2, t);
    auto s = slerpForSquad(s1, s2, t);
    return slerpForSquad(q, s, 2.0f * t * (1.0f - t));
}


Quaternion Quaternion::slerpForSquad(const Quaternion& q1, const Quaternion& q2, float t)
{
    auto c = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

    if (fabs(c) >= 1.0f)
        return q1;

    auto omega = acos(c);
    auto s = sqrt(1.0f - c * c);
    if (Math::approxZero(s))
        return q1;

    auto r1 = sin((1 - t) * omega) / s;
    auto r2 = sin(t * omega) / s;
    return Quaternion(
               q1.x * r1 + q2.x * r2,
               q1.y * r1 + q2.y * r2,
               q1.z * r1 + q2.z * r2,
               q1.w * r1 + q2.w * r2
           );
}


Quaternion& Quaternion::operator*=(const Quaternion& q)
{
    auto newX = w * q.x + x * q.w + y * q.z - z * q.y;
    auto newY = w * q.y - x * q.z + y * q.w + z * q.x;
    auto newZ = w * q.z + x * q.y - y * q.x + z * q.w;
    auto newW = w * q.w - x * q.x - y * q.y - z * q.z;
    x = newX;
    y = newY;
    z = newZ;
    w = newW;
    return *this;
}