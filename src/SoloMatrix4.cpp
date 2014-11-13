#include "SoloMatrix4.h"

using namespace solo;

const Matrix4 Matrix4::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);

const Matrix4 Matrix4::ZEROAFFINE(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 1);

const Matrix4 Matrix4::IDENTITY(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);

const Matrix4 Matrix4::CLIPSPACE2DTOIMAGESPACE(
	0.5, 0, 0, 0.5,
	0, -0.5, 0, 0.5,
	0, 0, 1, 0,
	0, 0, 0, 1);


inline static f32
MINOR(const Matrix4& m, const size_t r0, const size_t r1, const size_t r2,
const size_t c0, const size_t c1, const size_t c2)
{
	return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
		m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
		m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
}

Matrix4 Matrix4::adjoint() const
{
	return Matrix4(MINOR(*this, 1, 2, 3, 1, 2, 3),
		-MINOR(*this, 0, 2, 3, 1, 2, 3),
		MINOR(*this, 0, 1, 3, 1, 2, 3),
		-MINOR(*this, 0, 1, 2, 1, 2, 3),

		-MINOR(*this, 1, 2, 3, 0, 2, 3),
		MINOR(*this, 0, 2, 3, 0, 2, 3),
		-MINOR(*this, 0, 1, 3, 0, 2, 3),
		MINOR(*this, 0, 1, 2, 0, 2, 3),

		MINOR(*this, 1, 2, 3, 0, 1, 3),
		-MINOR(*this, 0, 2, 3, 0, 1, 3),
		MINOR(*this, 0, 1, 3, 0, 1, 3),
		-MINOR(*this, 0, 1, 2, 0, 1, 3),

		-MINOR(*this, 1, 2, 3, 0, 1, 2),
		MINOR(*this, 0, 2, 3, 0, 1, 2),
		-MINOR(*this, 0, 1, 3, 0, 1, 2),
		MINOR(*this, 0, 1, 2, 0, 1, 2));
}

f32 Matrix4::determinant() const
{
	return m[0][0] * MINOR(*this, 1, 2, 3, 1, 2, 3) -
		m[0][1] * MINOR(*this, 1, 2, 3, 0, 2, 3) +
		m[0][2] * MINOR(*this, 1, 2, 3, 0, 1, 3) -
		m[0][3] * MINOR(*this, 1, 2, 3, 0, 1, 2);
}

Matrix4 Matrix4::inverse() const
{
	f32 m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
	f32 m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
	f32 m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
	f32 m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

	f32 v0 = m20 * m31 - m21 * m30;
	f32 v1 = m20 * m32 - m22 * m30;
	f32 v2 = m20 * m33 - m23 * m30;
	f32 v3 = m21 * m32 - m22 * m31;
	f32 v4 = m21 * m33 - m23 * m31;
	f32 v5 = m22 * m33 - m23 * m32;

	f32 t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
	f32 t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
	f32 t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
	f32 t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

	f32 invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

	f32 d00 = t00 * invDet;
	f32 d10 = t10 * invDet;
	f32 d20 = t20 * invDet;
	f32 d30 = t30 * invDet;

	f32 d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	f32 d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	f32 d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	f32 d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m10 * m31 - m11 * m30;
	v1 = m10 * m32 - m12 * m30;
	v2 = m10 * m33 - m13 * m30;
	v3 = m11 * m32 - m12 * m31;
	v4 = m11 * m33 - m13 * m31;
	v5 = m12 * m33 - m13 * m32;

	f32 d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	f32 d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	f32 d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	f32 d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m21 * m10 - m20 * m11;
	v1 = m22 * m10 - m20 * m12;
	v2 = m23 * m10 - m20 * m13;
	v3 = m22 * m11 - m21 * m12;
	v4 = m23 * m11 - m21 * m13;
	v5 = m23 * m12 - m22 * m13;

	f32 d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	f32 d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	f32 d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	f32 d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	return Matrix4(
		d00, d01, d02, d03,
		d10, d11, d12, d13,
		d20, d21, d22, d23,
		d30, d31, d32, d33);
}

Matrix4 Matrix4::inverseAffine(void) const
{
	assert(isAffine());

	f32 m10 = m[1][0], m11 = m[1][1], m12 = m[1][2];
	f32 m20 = m[2][0], m21 = m[2][1], m22 = m[2][2];

	f32 t00 = m22 * m11 - m21 * m12;
	f32 t10 = m20 * m12 - m22 * m10;
	f32 t20 = m21 * m10 - m20 * m11;

	f32 m00 = m[0][0], m01 = m[0][1], m02 = m[0][2];

	f32 invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

	t00 *= invDet; t10 *= invDet; t20 *= invDet;

	m00 *= invDet; m01 *= invDet; m02 *= invDet;

	f32 r00 = t00;
	f32 r01 = m02 * m21 - m01 * m22;
	f32 r02 = m01 * m12 - m02 * m11;

	f32 r10 = t10;
	f32 r11 = m00 * m22 - m02 * m20;
	f32 r12 = m02 * m10 - m00 * m12;

	f32 r20 = t20;
	f32 r21 = m01 * m20 - m00 * m21;
	f32 r22 = m00 * m11 - m01 * m10;

	f32 m03 = m[0][3], m13 = m[1][3], m23 = m[2][3];

	f32 r03 = -(r00 * m03 + r01 * m13 + r02 * m23);
	f32 r13 = -(r10 * m03 + r11 * m13 + r12 * m23);
	f32 r23 = -(r20 * m03 + r21 * m13 + r22 * m23);

	return Matrix4(
		r00, r01, r02, r03,
		r10, r11, r12, r13,
		r20, r21, r22, r23,
		0, 0, 0, 1);
}

void Matrix4::makeTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation)
{
	// Ordering:
	//    1. Scale
	//    2. Rotate
	//    3. Translate

	Matrix3 rot3x3;
	orientation.ToRotationMatrix(rot3x3);

	// Set up final matrix with scale, rotation and translation
	m[0][0] = scale.x * rot3x3[0][0]; m[0][1] = scale.y * rot3x3[0][1]; m[0][2] = scale.z * rot3x3[0][2]; m[0][3] = position.x;
	m[1][0] = scale.x * rot3x3[1][0]; m[1][1] = scale.y * rot3x3[1][1]; m[1][2] = scale.z * rot3x3[1][2]; m[1][3] = position.y;
	m[2][0] = scale.x * rot3x3[2][0]; m[2][1] = scale.y * rot3x3[2][1]; m[2][2] = scale.z * rot3x3[2][2]; m[2][3] = position.z;

	// No projection term
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}

void Matrix4::makeInverseTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation)
{
	// Invert the parameters
	Vector3 invTranslate = -position;
	Vector3 invScale(1 / scale.x, 1 / scale.y, 1 / scale.z);
	Quaternion invRot = orientation.Inverse();

	// Because we're inverting, order is translation, rotation, scale
	// So make translation relative to scale & rotation
	invTranslate = invRot * invTranslate; // rotate
	invTranslate *= invScale; // scale

	// Next, make a 3x3 rotation matrix
	Matrix3 rot3x3;
	invRot.ToRotationMatrix(rot3x3);

	// Set up final matrix with scale, rotation and translation
	m[0][0] = invScale.x * rot3x3[0][0]; m[0][1] = invScale.x * rot3x3[0][1]; m[0][2] = invScale.x * rot3x3[0][2]; m[0][3] = invTranslate.x;
	m[1][0] = invScale.y * rot3x3[1][0]; m[1][1] = invScale.y * rot3x3[1][1]; m[1][2] = invScale.y * rot3x3[1][2]; m[1][3] = invTranslate.y;
	m[2][0] = invScale.z * rot3x3[2][0]; m[2][1] = invScale.z * rot3x3[2][1]; m[2][2] = invScale.z * rot3x3[2][2]; m[2][3] = invTranslate.z;

	// No projection term
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}

void Matrix4::decomposition(Vector3& position, Vector3& scale, Quaternion& orientation) const
{
	assert(isAffine());

	Matrix3 m3x3;
	extract3x3Matrix(m3x3);

	Matrix3 matQ;
	Vector3 vecU;
	m3x3.qduDecomposition(matQ, scale, vecU);

	orientation = Quaternion(matQ);
	position = Vector3(m[0][3], m[1][3], m[2][3]);
}