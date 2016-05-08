#include "SoloQuaternion.h"
#include "SoloMath.h"
#include "SoloRadian.h"

using namespace solo;


Quaternion::Quaternion(float x, float y, float z, float w):
    x(x), y(y), z(z), w(w)
{
}


Quaternion::Quaternion(const Vector3& axis, const Radian& angle)
{
    *this = createFromAxisAngle(axis, angle);
}


auto Quaternion::identity() -> Quaternion
{
    static Quaternion value(0.0f, 0.0f, 0.0f, 1.0f);
    return value;
}


auto Quaternion::zero() -> Quaternion
{
    static Quaternion value(0.0f, 0.0f, 0.0f, 0.0f);
    return value;
}


bool Quaternion::isIdentity() const
{
    return Math::approxZero(x, Math::smallFloat1) &&
           Math::approxZero(y, Math::smallFloat1) &&
           Math::approxZero(z, Math::smallFloat1) &&
           Math::approxEqual(w, 1.0f, Math::smallFloat1);
}


bool Quaternion::isZero() const
{
    return Math::approxZero(x, Math::smallFloat1) &&
           Math::approxZero(y, Math::smallFloat1) &&
           Math::approxZero(z, Math::smallFloat1) &&
           Math::approxZero(w, Math::smallFloat1);
}


auto Quaternion::createFromAxisAngle(const Vector3& axis, const Radian& angle) -> Quaternion
{
    auto halfAngle = angle.getRawRadians() * 0.5f;
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
    if (Math::approxEqual(n, 1.0f, Math::smallFloat1))
    {
        x = -x;
        y = -y;
        z = -z;
        return true;
    }

    if (Math::approxZero(n, Math::smallFloat1))
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
    if (Math::approxEqual(n, 1.0f, Math::smallFloat1))
        return;

    n = sqrt(n);
    if (Math::approxZero(n, Math::smallFloat1))
        return;

    n = 1.0f / n;
    x *= n;
    y *= n;
    z *= n;
    w *= n;
}


auto Quaternion::normalized() const -> Quaternion
{
    auto result(*this);
    result.normalize();
    return result;
}


auto Quaternion::toAxisAngle(Vector3& axis) const -> Radian
{
    Quaternion q(x, y, z, w);
    q.normalize();
    axis.x = q.x;
    axis.y = q.y;
    axis.z = q.z;
    axis.normalize();
    return Radian(2.0f * acos(q.w));
}


auto Quaternion::lerp(const Quaternion& q1, const Quaternion& q2, float t) -> Quaternion
{
    if (Math::approxZero(t, Math::smallFloat1))
        return q1;

    if (Math::approxEqual(t, 1.0f, Math::smallFloat1))
        return q2;

    auto t1 = 1.0f - t;

    return Quaternion(
        t1 * q1.x + t * q2.x,
        t1 * q1.y + t * q2.y,
        t1 * q1.z + t * q2.z,
        t1 * q1.w + t * q2.w);
}


auto Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, float t) -> Quaternion
{
    if (Math::approxZero(t, Math::smallFloat1))
        return q1;

    if (Math::approxEqual(t, 1.0f, Math::smallFloat1))
        return q2;

    if (Math::approxEqual(q1.x, q2.x, Math::smallFloat1) &&
        Math::approxEqual(q1.y, q2.y, Math::smallFloat1) &&
        Math::approxEqual(q1.z, q2.z, Math::smallFloat1) &&
        Math::approxEqual(q1.w, q2.w, Math::smallFloat1))
        return q1;

    auto cosTheta = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

    auto alpha = cosTheta >= 0 ? 1.0f : -1.0f;
    auto halfY = 1.0f + alpha * cosTheta;

    auto f2b = t - 0.5f;
    auto u = f2b >= 0 ? f2b : -f2b;
    auto f2a = u - f2b;
    f2b += u;
    u += u;
    auto f1 = 1.0f - u;

    auto halfSecHalfTheta = 1.09f - (0.476537f - 0.0903321f * halfY) * halfY;
    halfSecHalfTheta *= 1.5f - halfY * halfSecHalfTheta * halfSecHalfTheta;
    auto versHalfTheta = 1.0f - halfY * halfSecHalfTheta;

    auto sqNotU = f1 * f1;
    auto ratio2 = 0.0000440917108f * versHalfTheta;
    auto ratio1 = -0.00158730159f + (sqNotU - 16.0f) * ratio2;
    ratio1 = 0.0333333333f + ratio1 * (sqNotU - 9.0f) * versHalfTheta;
    ratio1 = -0.333333333f + ratio1 * (sqNotU - 4.0f) * versHalfTheta;
    ratio1 = 1.0f + ratio1 * (sqNotU - 1.0f) * versHalfTheta;

    auto sqU = u * u;
    ratio2 = -0.00158730159f + (sqU - 16.0f) * ratio2;
    ratio2 = 0.0333333333f + ratio2 * (sqU - 9.0f) * versHalfTheta;
    ratio2 = -0.333333333f + ratio2 * (sqU - 4.0f) * versHalfTheta;
    ratio2 = 1.0f + ratio2 * (sqU - 1.0f) * versHalfTheta;

    f1 *= ratio1 * halfSecHalfTheta;
    f2a *= ratio2;
    f2b *= ratio2;
    alpha *= f1 + f2a;
    auto beta = f1 + f2b;

    auto w = alpha * q1.w + beta * q2.w;
    auto x = alpha * q1.x + beta * q2.x;
    auto y = alpha * q1.y + beta * q2.y;
    auto z = alpha * q1.z + beta * q2.z;

    // Quaternion length correction, if needed
    f1 = 1.5f - 0.5f * (w * w + x * x + y * y + z * z);
    return Quaternion(w * f1, x * f1, y * f1, z * f1);
}


auto Quaternion::squad(const Quaternion& q1, const Quaternion& q2, const Quaternion& s1, const Quaternion& s2, float t) -> Quaternion
{
    auto q = slerpForSquad(q1, q2, t);
    auto s = slerpForSquad(s1, s2, t);
    return slerpForSquad(q, s, 2.0f * t * (1.0f - t));
}


auto Quaternion::slerpForSquad(const Quaternion& q1, const Quaternion& q2, float t) -> Quaternion
{
    auto c = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

    if (fabs(c) >= 1.0f)
        return q1;

    auto omega = acos(c);
    auto s = sqrt(1.0f - c * c);
    if (Math::approxZero(s, Math::smallFloat1))
        return q1;

    auto r1 = sin((1 - t) * omega) / s;
    auto r2 = sin(t * omega) / s;
    return Quaternion(
        q1.x * r1 + q2.x * r2,
        q1.y * r1 + q2.y * r2,
        q1.z * r1 + q2.z * r2,
        q1.w * r1 + q2.w * r2);
}


auto Quaternion::operator*=(const Quaternion& q) -> Quaternion&
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