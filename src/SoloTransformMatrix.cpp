#include "SoloTransformMatrix.h"
#include "SoloCommon.h"
#include "SoloMath.h"
#include "SoloPlane.h"
#include "SoloQuaternion.h"
#include "SoloBoundingBox.h"
#include "SoloBoundingSphere.h"
#include "SoloRay.h"
#include <algorithm>

using namespace solo;


TransformMatrix::TransformMatrix()
{
}


TransformMatrix::TransformMatrix(const TransformMatrix& other):
    Matrix(other)
{
}


TransformMatrix::TransformMatrix(const Matrix& m):
    Matrix(m)
{
}


auto TransformMatrix::createLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) -> TransformMatrix
{
    auto zaxis(eye - target);
    zaxis.normalize();

    auto xaxis = Vector3::cross(up.normalized(), zaxis);
    xaxis.normalize();

    auto yaxis = Vector3::cross(zaxis, xaxis);
    yaxis.normalize();

    // Matrix is built already transposed
    return TransformMatrix(Matrix(
        xaxis.x, yaxis.x, zaxis.x, 0,
        xaxis.y, yaxis.y, zaxis.y, 0,
        xaxis.z, yaxis.z, zaxis.z, 0,
        -Vector3::dot(xaxis, eye), -Vector3::dot(yaxis, eye), -Vector3::dot(zaxis, eye), 1
    ));
}


auto TransformMatrix::createPerspective(const Radian& fieldOfView, float aspectRatio, float znear, float zfar) -> TransformMatrix
{
    auto f_n = 1.0f / (zfar - znear);
    auto theta = fieldOfView.toRawRadian() * 0.5f;
    SL_ERR_IF(Math::approxZero(fmod(theta, Math::piOver2), Math::smallFloat1))

    auto divisor = tan(theta);
    auto factor = 1.0f / divisor;

    Matrix result;
    memset(&result.m, 0, Matrix::Size);
    result.m[0] = (1.0f / aspectRatio) * factor;
    result.m[5] = factor;
    result.m[10] = -(zfar + znear) * f_n;
    result.m[11] = -1.0f;
    result.m[14] = -2.0f * zfar * znear * f_n;

    return TransformMatrix(result);
}


auto TransformMatrix::createOrthographic(float width, float height, float near, float far) -> TransformMatrix
{
    auto halfWidth = width / 2.0f;
    auto halfHeight = height / 2.0f;
    auto left = -halfWidth;
    auto right = halfWidth;
    auto top = halfHeight;
    auto bottom = -halfHeight;
    Matrix result;
    memset(&result.m, 0, Matrix::Size);
    result.m[0] = 2 / (right - left);
    result.m[5] = 2 / (top - bottom);
    result.m[12] = (left + right) / (left - right);
    result.m[10] = 1 / (near - far);
    result.m[13] = (top + bottom) / (bottom - top);
    result.m[14] = near / (near - far);
    result.m[15] = 1;
    return TransformMatrix(result);
}


auto TransformMatrix::createReflection(const Plane& plane) -> TransformMatrix
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

    return TransformMatrix(result);
}


auto TransformMatrix::createScale(const Vector3& scale) -> TransformMatrix
{
    Matrix result;
    result.m[0] = scale.x;
    result.m[5] = scale.y;
    result.m[10] = scale.z;
    return TransformMatrix(result);
}


auto TransformMatrix::createRotationFromQuaternion(const Quaternion& q) -> TransformMatrix
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

    return TransformMatrix(result);
}


auto TransformMatrix::createRotationFromAxisAngle(const Vector3& axis, const Radian& angle) -> TransformMatrix
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

    return TransformMatrix(result);
}


auto TransformMatrix::createRotationX(const Radian& angle) -> TransformMatrix
{
    auto c = cos(angle.toRawRadian());
    auto s = sin(angle.toRawRadian());

    Matrix result;
    result.m[5] = c;
    result.m[6] = s;
    result.m[9] = -s;
    result.m[10] = c;
    return TransformMatrix(result);
}


auto TransformMatrix::createRotationY(const Radian& angle) -> TransformMatrix
{
    auto c = cos(angle.toRawRadian());
    auto s = sin(angle.toRawRadian());

    Matrix result;

    result.m[0] = c;
    result.m[2] = -s;
    result.m[8] = s;
    result.m[10] = c;

    return TransformMatrix(result);
}


auto TransformMatrix::createRotationZ(const Radian& angle) -> TransformMatrix
{
    auto c = cos(angle.toRawRadian());
    auto s = sin(angle.toRawRadian());

    Matrix result;
    result.m[0] = c;
    result.m[1] = s;
    result.m[4] = -s;
    result.m[5] = c;
    return TransformMatrix(result);
}


auto TransformMatrix::createTranslation(const Vector3& translation) -> TransformMatrix
{
    Matrix result;
    result.m[12] = translation.x;
    result.m[13] = translation.y;
    result.m[14] = translation.z;
    return TransformMatrix(result);
}


auto TransformMatrix::getScale() const -> Vector3
{
    Vector3 result;
    decompose(&result, nullptr, nullptr);
    return result;
}


auto TransformMatrix::getRotation() const -> Quaternion
{
    Quaternion result;
    decompose(nullptr, &result, nullptr);
    return result;
}


auto TransformMatrix::getTranslation() const -> Vector3
{
    Vector3 result;
    decompose(nullptr, nullptr, &result);
    return result;
}


void TransformMatrix::scaleByVector(const Vector3& s)
{
    *this *= createScale(s);
}


auto TransformMatrix::transformPoint(const Vector3& point) const -> Vector3
{
    return Vector3(
        point.x * m[0] + point.y * m[4] + point.z * m[8] + m[12],
        point.x * m[1] + point.y * m[5] + point.z * m[9] + m[13],
        point.x * m[2] + point.y * m[6] + point.z * m[10] + m[14]
    );
}


auto TransformMatrix::transformDirection(const Vector3& dir) const -> Vector3
{
    return Vector3(
        dir.x * m[0] + dir.y * m[4] + dir.z * m[8],
        dir.x * m[1] + dir.y * m[5] + dir.z * m[9],
        dir.x * m[2] + dir.y * m[6] + dir.z * m[10]
    );
}


void TransformMatrix::translate(const Vector3& t)
{
    *this *= createTranslation(t);
}


static void updateBounds(const Vector3& point, Vector3& min, Vector3& max)
{
    if (point.x < min.x)
        min.x = point.x;

    if (point.x > max.x)
        max.x = point.x;

    if (point.y < min.y)
        min.y = point.y;

    if (max.y < point.y)
        max.y = point.y;

    if (min.z > point.z)
        min.z = point.z;

    if (point.z > max.z)
        max.z = point.z;
}


auto TransformMatrix::transformBoundingBox(const BoundingBox& box) -> BoundingBox
{
    auto corners = box.getCorners();
	auto newMin = Vector3(std::numeric_limits<float>::max());
	auto newMax = Vector3(std::numeric_limits<float>::min());

	for (const auto& corner: corners)
    {
        auto newCorner = transformPoint(corner);
        updateBounds(newCorner, newMin, newMax);
    }

    return BoundingBox(
        Vector3(newMin.x, newMin.y, newMin.z),
        Vector3(newMax.x, newMax.y, newMax.z));
}


auto TransformMatrix::transformBoundingSphere(const BoundingSphere& sphere) -> BoundingSphere
{
    auto scale = getScale();
    auto r = sphere.radius * scale.x;
    r = std::max(r, sphere.radius * scale.y);
    r = std::max(r, sphere.radius * scale.z);
    return BoundingSphere(transformPoint(sphere.center), r);
}


auto TransformMatrix::transformPlane(const Plane& plane) -> Plane
{
    auto inverted(*this);
    if (!inverted.invert())
        return plane;

    auto normal = plane.getNormal();
    auto distance = plane.getDistance();

    // Treat the plane as a four-tuple and multiply by the inverse transpose of the matrix to get the transformed plane.
    // Then we normalize the plane by dividing both the normal and the distance by the length of the normal.
    auto nx = normal.x * inverted.m[0] + normal.y * inverted.m[1] + normal.z * inverted.m[2] + distance * inverted.m[3];
    auto ny = normal.x * inverted.m[4] + normal.y * inverted.m[5] + normal.z * inverted.m[6] + distance * inverted.m[7];
    auto nz = normal.x * inverted.m[8] + normal.y * inverted.m[9] + normal.z * inverted.m[10] + distance * inverted.m[11];
    auto d = normal.x * inverted.m[12] + normal.y * inverted.m[13] + normal.z * inverted.m[14] + distance * inverted.m[15];
    auto divisor = sqrt(nx * nx + ny * ny + nz * nz);
    auto factor = 1.0f / divisor;

    normal.x = nx * factor;
    normal.y = ny * factor;
    normal.z = nz * factor;

    return Plane(normal, d * factor);
}


auto TransformMatrix::transformRay(const Ray& ray) -> Ray
{
    auto origin = transformPoint(ray.getOrigin());
    auto direction = transformDirection(ray.getDirection());
    direction.normalize();
    return Ray(origin, direction);
}


bool TransformMatrix::decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const
{
    if (translation)
    {
        translation->x = m[12];
        translation->y = m[13];
        translation->z = m[14];
    }

    if (scale == nullptr && rotation == nullptr)
        return true;

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
