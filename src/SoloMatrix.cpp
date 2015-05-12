#include "SoloMatrix.h"
#include "SoloMath.h"
#include "SoloQuaternion.h"
#include "SoloPlane.h"
#include "SoloException.h"

using namespace solo;


static const float MATRIX_IDENTITY[16] =
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};


Matrix::Matrix()
{
	*this = Matrix::identity();
}


Matrix::Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
	set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
}


Matrix::Matrix(const float* m)
{
	set(m);
}


Matrix::Matrix(const Matrix& copy)
{
	memcpy(m, copy.m, MATRIX_SIZE);
}


Matrix::~Matrix()
{
}


const Matrix& Matrix::identity()
{
	static Matrix m(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	return m;
}


const Matrix& Matrix::zero()
{
	static Matrix m(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);
	return m;
}


void Matrix::createLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up, Matrix* dst)
{
	createLookAt(eyePosition.x, eyePosition.y, eyePosition.z, targetPosition.x, targetPosition.y, targetPosition.z,
		up.x, up.y, up.z, dst);
}


void Matrix::createLookAt(float eyePositionX, float eyePositionY, float eyePositionZ,
	float targetPositionX, float targetPositionY, float targetPositionZ, float upX, float upY, float upZ, Matrix* dst)
{
	Vector3 eye(eyePositionX, eyePositionY, eyePositionZ);
	Vector3 target(targetPositionX, targetPositionY, targetPositionZ);
	Vector3 up(upX, upY, upZ);
	up.normalize();

	Vector3 zaxis;
	Vector3::subtract(eye, target, &zaxis);
	zaxis.normalize();

	Vector3 xaxis;
	Vector3::cross(up, zaxis, &xaxis);
	xaxis.normalize();

	Vector3 yaxis;
	Vector3::cross(zaxis, xaxis, &yaxis);
	yaxis.normalize();

	dst->m[0] = xaxis.x;
	dst->m[1] = yaxis.x;
	dst->m[2] = zaxis.x;
	dst->m[3] = 0.0f;

	dst->m[4] = xaxis.y;
	dst->m[5] = yaxis.y;
	dst->m[6] = zaxis.y;
	dst->m[7] = 0.0f;

	dst->m[8] = xaxis.z;
	dst->m[9] = yaxis.z;
	dst->m[10] = zaxis.z;
	dst->m[11] = 0.0f;

	dst->m[12] = -Vector3::dot(xaxis, eye);
	dst->m[13] = -Vector3::dot(yaxis, eye);
	dst->m[14] = -Vector3::dot(zaxis, eye);
	dst->m[15] = 1.0f;
}


void Matrix::createPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane, Matrix* dst)
{
	auto f_n = 1.0f / (zFarPlane - zNearPlane);
	auto theta = MATH_DEG_TO_RAD(fieldOfView) * 0.5f;
	if (fabs(fmod(theta, MATH_PIOVER2)) < MATH_EPSILON)
		THROW_FMT(EngineException, "Invalid field of view value ", fieldOfView, " caused attempted tan calculation, which is undefined");
	auto divisor = tan(theta);
	auto factor = 1.0f / divisor;

	memset(dst, 0, MATRIX_SIZE);

	dst->m[0] = (1.0f / aspectRatio) * factor;
	dst->m[5] = factor;
	dst->m[10] = (-(zFarPlane + zNearPlane)) * f_n;
	dst->m[11] = -1.0f;
	dst->m[14] = -2.0f * zFarPlane * zNearPlane * f_n;
}


void Matrix::createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Matrix* dst)
{
	auto halfWidth = width / 2.0f;
	auto halfHeight = height / 2.0f;
	createOrthographicOffCenter(-halfWidth, halfWidth, -halfHeight, halfHeight, zNearPlane, zFarPlane, dst);
}


void Matrix::createOrthographicOffCenter(float left, float right, float bottom, float top, float near, float far, Matrix* dst)
{
	memset(dst, 0, MATRIX_SIZE);
	dst->m[0] = 2 / (right - left);
	dst->m[5] = 2 / (top - bottom);
	dst->m[12] = (left + right) / (left - right);
	dst->m[10] = 1 / (near - far);
	dst->m[13] = (top + bottom) / (bottom - top);
	dst->m[14] = near / (near - far);
	dst->m[15] = 1;
}


void Matrix::createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition, const Vector3& cameraUpVector, Matrix* dst)
{
	createBillboardHelper(objectPosition, cameraPosition, cameraUpVector, nullptr, dst);
}


void Matrix::createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition,
	const Vector3& cameraUpVector, const Vector3& cameraForwardVector, Matrix* dst)
{
	createBillboardHelper(objectPosition, cameraPosition, cameraUpVector, &cameraForwardVector, dst);
}


void Matrix::createBillboardHelper(const Vector3& objectPosition, const Vector3& cameraPosition,
	const Vector3& cameraUpVector, const Vector3* cameraForwardVector, Matrix* dst)
{
	Vector3 delta(objectPosition, cameraPosition);
	auto isSufficientDelta = delta.lengthSquared() > MATH_EPSILON;

	dst->setIdentity();
	dst->m[3] = objectPosition.x;
	dst->m[7] = objectPosition.y;
	dst->m[11] = objectPosition.z;

	// As per the contracts for the 2 variants of createBillboard, we need
	// either a safe default or a sufficient distance between object and camera.
	if (cameraForwardVector || isSufficientDelta)
	{
		auto target = isSufficientDelta ? cameraPosition : (objectPosition - *cameraForwardVector);

		// A billboard is the inverse of a lookAt rotation
		Matrix lookAt;
		createLookAt(objectPosition, target, cameraUpVector, &lookAt);
		dst->m[0] = lookAt.m[0];
		dst->m[1] = lookAt.m[4];
		dst->m[2] = lookAt.m[8];
		dst->m[4] = lookAt.m[1];
		dst->m[5] = lookAt.m[5];
		dst->m[6] = lookAt.m[9];
		dst->m[8] = lookAt.m[2];
		dst->m[9] = lookAt.m[6];
		dst->m[10] = lookAt.m[10];
	}
}


void Matrix::createReflection(const Plane& plane, Matrix* dst)
{
	auto normal(plane.getNormal());
	auto k = -2.0f * plane.getDistance();

	dst->setIdentity();

	dst->m[0] -= 2.0f * normal.x * normal.x;
	dst->m[5] -= 2.0f * normal.y * normal.y;
	dst->m[10] -= 2.0f * normal.z * normal.z;
	dst->m[1] = dst->m[4] = -2.0f * normal.x * normal.y;
	dst->m[2] = dst->m[8] = -2.0f * normal.x * normal.z;
	dst->m[6] = dst->m[9] = -2.0f * normal.y * normal.z;

	dst->m[3] = k * normal.x;
	dst->m[7] = k * normal.y;
	dst->m[11] = k * normal.z;
}


void Matrix::createScale(const Vector3& scale, Matrix* dst)
{
	memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

	dst->m[0] = scale.x;
	dst->m[5] = scale.y;
	dst->m[10] = scale.z;
}


void Matrix::createScale(float xScale, float yScale, float zScale, Matrix* dst)
{
	memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

	dst->m[0] = xScale;
	dst->m[5] = yScale;
	dst->m[10] = zScale;
}


void Matrix::createRotation(const Quaternion& q, Matrix* dst)
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

	dst->m[0] = 1.0f - yy2 - zz2;
	dst->m[1] = xy2 + wz2;
	dst->m[2] = xz2 - wy2;
	dst->m[3] = 0.0f;

	dst->m[4] = xy2 - wz2;
	dst->m[5] = 1.0f - xx2 - zz2;
	dst->m[6] = yz2 + wx2;
	dst->m[7] = 0.0f;

	dst->m[8] = xz2 + wy2;
	dst->m[9] = yz2 - wx2;
	dst->m[10] = 1.0f - xx2 - yy2;
	dst->m[11] = 0.0f;

	dst->m[12] = 0.0f;
	dst->m[13] = 0.0f;
	dst->m[14] = 0.0f;
	dst->m[15] = 1.0f;
}


void Matrix::createRotation(const Vector3& axis, float angleRadians, Matrix* dst)
{
	auto x = axis.x;
	auto y = axis.y;
	auto z = axis.z;

	// Make sure the input axis is normalized.
	auto n = x*x + y*y + z*z;
	if (n != 1.0f)
	{
		// Not normalized.
		n = sqrt(n);
		// Prevent divide too close to zero.
		if (n > 0.000001f)
		{
			n = 1.0f / n;
			x *= n;
			y *= n;
			z *= n;
		}
	}

	auto c = cos(angleRadians);
	auto s = sin(angleRadians);

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

	dst->m[0] = c + tx*x;
	dst->m[1] = txy + sz;
	dst->m[2] = txz - sy;
	dst->m[3] = 0.0f;

	dst->m[4] = txy - sz;
	dst->m[5] = c + ty*y;
	dst->m[6] = tyz + sx;
	dst->m[7] = 0.0f;

	dst->m[8] = txz + sy;
	dst->m[9] = tyz - sx;
	dst->m[10] = c + tz*z;
	dst->m[11] = 0.0f;

	dst->m[12] = 0.0f;
	dst->m[13] = 0.0f;
	dst->m[14] = 0.0f;
	dst->m[15] = 1.0f;
}


void Matrix::createRotationX(float angleRadians, Matrix* dst)
{
	memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

	auto c = cos(angleRadians);
	auto s = sin(angleRadians);

	dst->m[5] = c;
	dst->m[6] = s;
	dst->m[9] = -s;
	dst->m[10] = c;
}


void Matrix::createRotationY(float angleRadians, Matrix* dst)
{
	memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

	auto c = cos(angleRadians);
	auto s = sin(angleRadians);

	dst->m[0] = c;
	dst->m[2] = -s;
	dst->m[8] = s;
	dst->m[10] = c;
}


void Matrix::createRotationZ(float angleRadians, Matrix* dst)
{
	memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

	auto c = cos(angleRadians);
	auto s = sin(angleRadians);

	dst->m[0] = c;
	dst->m[1] = s;
	dst->m[4] = -s;
	dst->m[5] = c;
}


void Matrix::createTranslation(const Vector3& translation, Matrix* dst)
{
	memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

	dst->m[12] = translation.x;
	dst->m[13] = translation.y;
	dst->m[14] = translation.z;
}


void Matrix::createTranslation(float xTranslation, float yTranslation, float zTranslation, Matrix* dst)
{
	memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

	dst->m[12] = xTranslation;
	dst->m[13] = yTranslation;
	dst->m[14] = zTranslation;
}


void Matrix::add(float scalar)
{
	add(scalar, this);
}


void Matrix::add(float scalar, Matrix* dst)
{
	Math::addMatrix(m, scalar, dst->m);
}


void Matrix::add(const Matrix& m)
{
	add(*this, m, this);
}


void Matrix::add(const Matrix& m1, const Matrix& m2, Matrix* dst)
{
	Math::addMatrix(m1.m, m2.m, dst->m);
}


bool Matrix::decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const
{
	if (translation)
	{
		translation->x = m[12];
		translation->y = m[13];
		translation->z = m[14];
	}

	// Nothing left to do.
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
	auto det = determinant();
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
	if (scaleX < MATH_TOLERANCE || scaleY < MATH_TOLERANCE || fabs(scaleZ) < MATH_TOLERANCE)
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

	// Now calculate the rotation from the resulting matrix (axes).
	auto trace = xaxis.x + yaxis.y + zaxis.z + 1.0f;

	if (trace > MATH_EPSILON)
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


float Matrix::determinant() const
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

	// Calculate the determinant.
	return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
}


void Matrix::getScale(Vector3* scale) const
{
	decompose(scale, nullptr, nullptr);
}


Vector3 Matrix::getScale() const
{
	Vector3 result;
	getScale(&result);
	return result;
}


bool Matrix::getRotation(Quaternion* rotation) const
{
	return decompose(nullptr, rotation, nullptr);
}


Quaternion Matrix::getRotation() const
{
	Quaternion result;
	getRotation(&result);
	return result;
}


void Matrix::getTranslation(Vector3* translation) const
{
	decompose(nullptr, nullptr, translation);
}


Vector3 Matrix::getTranslation() const
{
	Vector3 result;
	getTranslation(&result);
	return result;
}


void Matrix::getUpVector(Vector3* dst) const
{
	dst->x = m[4];
	dst->y = m[5];
	dst->z = m[6];
}


void Matrix::getDownVector(Vector3* dst) const
{
	dst->x = -m[4];
	dst->y = -m[5];
	dst->z = -m[6];
}


void Matrix::getLeftVector(Vector3* dst) const
{
	dst->x = -m[0];
	dst->y = -m[1];
	dst->z = -m[2];
}


void Matrix::getRightVector(Vector3* dst) const
{
	dst->x = m[0];
	dst->y = m[1];
	dst->z = m[2];
}


void Matrix::getForwardVector(Vector3* dst) const
{
	dst->x = -m[8];
	dst->y = -m[9];
	dst->z = -m[10];
}


void Matrix::getBackVector(Vector3* dst) const
{
	dst->x = m[8];
	dst->y = m[9];
	dst->z = m[10];
}


Vector3 Matrix::getUpVector() const
{
	Vector3 result;
	getUpVector(&result);
	return result;
}


Vector3 Matrix::getDownVector() const
{
	Vector3 result;
	getDownVector(&result);
	return result;
}


Vector3 Matrix::getLeftVector() const
{
	Vector3 result;
	getLeftVector(&result);
	return result;
}


Vector3 Matrix::getRightVector() const
{
	Vector3 result;
	getRightVector(&result);
	return result;
}


Vector3 Matrix::getForwardVector() const
{
	Vector3 result;
	getForwardVector(&result);
	return result;
}


Vector3 Matrix::getBackVector() const
{
	Vector3 result;
	getBackVector(&result);
	return result;
}


bool Matrix::invert()
{
	return invert(this);
}


bool Matrix::invert(Matrix* dst) const
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

	// Calculate the determinant.
	auto det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

	// Close to zero, can't invert.
	if (fabs(det) <= MATH_TOLERANCE)
		return false;

	// Support the case where m == dst.
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

	multiply(inverse, 1.0f / det, dst);

	return true;
}


bool Matrix::isIdentity() const
{
	return (memcmp(m, MATRIX_IDENTITY, MATRIX_SIZE) == 0);
}


void Matrix::multiply(const Matrix& m, float scalar, Matrix* dst)
{
	Math::multiplyMatrix(m.m, scalar, dst->m);
}


void Matrix::multiply(const Matrix& m1, const Matrix& m2, Matrix* dst)
{
	Math::multiplyMatrix(m1.m, m2.m, dst->m);
}


void Matrix::negate()
{
	negate(this);
}


void Matrix::negate(Matrix* dst) const
{
	Math::negateMatrix(m, dst->m);
}


void Matrix::rotate(const Quaternion& q)
{
	rotate(q, this);
}


void Matrix::rotate(const Quaternion& q, Matrix* dst) const
{
	Matrix r;
	createRotation(q, &r);
	multiply(*this, r, dst);
}


void Matrix::rotate(const Vector3& axis, float angleRadians)
{
	rotate(axis, angleRadians, this);
}


void Matrix::rotate(const Vector3& axis, float angleRadians, Matrix* dst) const
{
	Matrix r;
	createRotation(axis, angleRadians, &r);
	multiply(*this, r, dst);
}


void Matrix::rotateX(float angleRadians)
{
	rotateX(angleRadians, this);
}


void Matrix::rotateX(float angleRadians, Matrix* dst) const
{
	Matrix r;
	createRotationX(angleRadians, &r);
	multiply(*this, r, dst);
}


void Matrix::rotateY(float angleRadians)
{
	rotateY(angleRadians, this);
}


void Matrix::rotateY(float angleRadians, Matrix* dst) const
{
	Matrix r;
	createRotationY(angleRadians, &r);
	multiply(*this, r, dst);
}


void Matrix::rotateZ(float angleRadians)
{
	rotateZ(angleRadians, this);
}


void Matrix::rotateZ(float angleRadians, Matrix* dst) const
{
	Matrix r;
	createRotationZ(angleRadians, &r);
	multiply(*this, r, dst);
}


void Matrix::scale(float value)
{
	scale(value, this);
}


void Matrix::scale(float value, Matrix* dst) const
{
	scale(value, value, value, dst);
}


void Matrix::scale(float xScale, float yScale, float zScale)
{
	scale(xScale, yScale, zScale, this);
}


void Matrix::scale(float xScale, float yScale, float zScale, Matrix* dst) const
{
	Matrix s;
	createScale(xScale, yScale, zScale, &s);
	multiply(*this, s, dst);
}


void Matrix::scale(const Vector3& s)
{
	scale(s.x, s.y, s.z, this);
}


void Matrix::scale(const Vector3& s, Matrix* dst) const
{
	scale(s.x, s.y, s.z, dst);
}


void Matrix::set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
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


void Matrix::set(const float* m)
{
	memcpy(this->m, m, MATRIX_SIZE);
}


void Matrix::set(const Matrix& m)
{
	memcpy(this->m, m.m, MATRIX_SIZE);
}


void Matrix::setIdentity()
{
	memcpy(m, MATRIX_IDENTITY, MATRIX_SIZE);
}


void Matrix::setZero()
{
	memset(m, 0, MATRIX_SIZE);
}


void Matrix::subtract(const Matrix& m)
{
	subtract(*this, m, this);
}


void Matrix::subtract(const Matrix& m1, const Matrix& m2, Matrix* dst)
{
	Math::subtractMatrix(m1.m, m2.m, dst->m);
}


void Matrix::transformPoint(Vector3* point) const
{
	transformDirection(point->x, point->y, point->z, 1.0f, point);
}


void Matrix::transformPoint(const Vector3& point, Vector3* dst) const
{
	transformDirection(point.x, point.y, point.z, 1.0f, dst);
}


Vector3 Matrix::transformPoint(const Vector3& point) const
{
	Vector3 result;
	transformPoint(point, &result);
	return result;
}


void Matrix::transformDirection(Vector3* dir) const
{
	transformDirection(dir->x, dir->y, dir->z, 0.0f, dir);
}


void Matrix::transformDirection(const Vector3& dir, Vector3* dst) const
{
	transformDirection(dir.x, dir.y, dir.z, 0.0f, dst);
}


void Matrix::transformDirection(float x, float y, float z, float w, Vector3* dst) const
{
	Math::transformVector4(m, x, y, z, w, reinterpret_cast<float*>(dst));
}


void Matrix::transformDirection(Vector4* dir) const
{
	transformDirection(*dir, dir);
}


void Matrix::transformDirection(const Vector4& dir, Vector4* dst) const
{
	Math::transformVector4(m, reinterpret_cast<const float*>(&dir), reinterpret_cast<float*>(dst));
}


Vector3 Matrix::transformDirection(const Vector3& direction) const
{
	Vector3 result;
	transformDirection(direction, &result);
	return result;
}


void Matrix::translate(float x, float y, float z)
{
	translate(x, y, z, this);
}


void Matrix::translate(float x, float y, float z, Matrix* dst) const
{
	Matrix t;
	createTranslation(x, y, z, &t);
	multiply(*this, t, dst);
}


void Matrix::translate(const Vector3& t)
{
	translate(t.x, t.y, t.z, this);
}


void Matrix::translate(const Vector3& t, Matrix* dst) const
{
	translate(t.x, t.y, t.z, dst);
}


void Matrix::transpose()
{
	transpose(this);
}


void Matrix::transpose(Matrix* dst) const
{
	Math::transposeMatrix(m, dst->m);
}
