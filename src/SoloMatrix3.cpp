#include "SoloMatrix3.h"
#include "SoloVector3.h"

using namespace solo;

const f32 Matrix3::EPSILON = 1e-06;
const Matrix3 Matrix3::ZERO(0, 0, 0, 0, 0, 0, 0, 0, 0);
const Matrix3 Matrix3::IDENTITY(1, 0, 0, 0, 1, 0, 0, 0, 1);

const f32 Matrix3::_svdEpsilon = 1e-04;
const unsigned int Matrix3::_svdMaxIterations = 32;


Vector3 Matrix3::column(size_t i) const
{
	assert(i < 3);
	return Vector3(m[0][i], m[1][i], m[2][i]);
}


void Matrix3::setColumn(size_t i, const Vector3& vec)
{
	assert(i < 3);
	m[0][i] = vec.x;
	m[1][i] = vec.y;
	m[2][i] = vec.z;
}


void Matrix3::fromAxes(const Vector3& x, const Vector3& y, const Vector3& z)
{
	setColumn(0, x);
	setColumn(1, y);
	setColumn(2, z);
}


bool Matrix3::operator==(const Matrix3& other) const
{
	for (size_t iRow = 0; iRow < 3; iRow++)
	{
		for (size_t iCol = 0; iCol < 3; iCol++)
		{
			if (m[iRow][iCol] != other.m[iRow][iCol])
				return false;
		}
	}

	return true;
}


Matrix3 Matrix3::operator+(const Matrix3& other) const
{
	Matrix3 result;
	for (size_t iRow = 0; iRow < 3; iRow++)
	{
		for (size_t iCol = 0; iCol < 3; iCol++)
			result.m[iRow][iCol] = m[iRow][iCol] + other.m[iRow][iCol];
	}
	return result;
}


Matrix3 Matrix3::operator-(const Matrix3& other) const
{
	Matrix3 result;
	for (size_t iRow = 0; iRow < 3; iRow++)
	{
		for (size_t iCol = 0; iCol < 3; iCol++)
			result.m[iRow][iCol] = m[iRow][iCol] - other.m[iRow][iCol];
	}
	return result;
}


Matrix3 Matrix3::operator*(const Matrix3& other) const
{
	Matrix3 result;
	for (size_t iRow = 0; iRow < 3; iRow++)
	{
		for (size_t iCol = 0; iCol < 3; iCol++)
		{
			result.m[iRow][iCol] =
				m[iRow][0] * other.m[0][iCol] +
				m[iRow][1] * other.m[1][iCol] +
				m[iRow][2] * other.m[2][iCol];
		}
	}
	return result;
}


Vector3 Matrix3::operator*(const Vector3& vector) const
{
	Vector3 kProd;
	for (size_t iRow = 0; iRow < 3; iRow++)
	{
		kProd[iRow] =
			m[iRow][0] * vector[0] +
			m[iRow][1] * vector[1] +
			m[iRow][2] * vector[2];
	}
	return kProd;
}


namespace solo
{
	Vector3 operator*(const Vector3& vector, const Matrix3& matrix)
	{
		Vector3 result;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			result[iRow] =
				vector[0] * matrix.m[0][iRow] +
				vector[1] * matrix.m[1][iRow] +
				vector[2] * matrix.m[2][iRow];
		}
		return result;
	}


	Matrix3 operator*(f32 fScalar, const Matrix3& rkMatrix)
	{
		Matrix3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				kProd[iRow][iCol] = fScalar * rkMatrix.m[iRow][iCol];
		}
		return kProd;
	}
}


Matrix3 Matrix3::operator-() const
{
	Matrix3 result;
	for (size_t iRow = 0; iRow < 3; iRow++)
	{
		for (size_t iCol = 0; iCol < 3; iCol++)
			result[iRow][iCol] = -m[iRow][iCol];
	}
	return result;
}


Matrix3 Matrix3::operator*(f32 fScalar) const
{
	Matrix3 kProd;
	for (size_t iRow = 0; iRow < 3; iRow++)
	{
		for (size_t iCol = 0; iCol < 3; iCol++)
			kProd[iRow][iCol] = fScalar * m[iRow][iCol];
	}
	return kProd;
}


Matrix3 Matrix3::transpose() const
{
	Matrix3 kTranspose;
	for (size_t iRow = 0; iRow < 3; iRow++)
	{
		for (size_t iCol = 0; iCol < 3; iCol++)
			kTranspose[iRow][iCol] = m[iCol][iRow];
	}
	return kTranspose;
}


bool Matrix3::inverse(Matrix3& inverse, f32 tolerance) const
{
	// Invert a 3x3 using cofactors.  This is about 8 times faster than
	// the Numerical Recipes code which uses Gaussian elimination.

	inverse[0][0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
	inverse[0][1] = m[0][2] * m[2][1] - m[0][1] * m[2][2];
	inverse[0][2] = m[0][1] * m[1][2] - m[0][2] * m[1][1];
	inverse[1][0] = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	inverse[1][1] = m[0][0] * m[2][2] - m[0][2] * m[2][0];
	inverse[1][2] = m[0][2] * m[1][0] - m[0][0] * m[1][2];
	inverse[2][0] = m[1][0] * m[2][1] - m[1][1] * m[2][0];
	inverse[2][1] = m[0][1] * m[2][0] - m[0][0] * m[2][1];
	inverse[2][2] = m[0][0] * m[1][1] - m[0][1] * m[1][0];

	f32 det =
		m[0][0] * inverse[0][0] +
		m[0][1] * inverse[1][0] +
		m[0][2] * inverse[2][0];

	if (Math::abs(det) <= tolerance)
		return false;

	f32 invDet = 1.0f / det;
	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
			inverse[row][col] *= invDet;
	}

	return true;
}

Matrix3 Matrix3::inverse(f32 tolerance) const
{
	Matrix3 result = ZERO;
	inverse(result, tolerance);
	return result;
}

f32 Matrix3::determinant() const
{
	f32 cofactor00 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
	f32 cofactor10 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	f32 cofactor20 = m[1][0] * m[2][1] - m[1][1] * m[2][0];

	f32 det =
		m[0][0] * cofactor00 +
		m[0][1] * cofactor10 +
		m[0][2] * cofactor20;

	return det;
}

void Matrix3::_bidiagonalize(Matrix3& a, Matrix3& l, Matrix3& r)
{
	f32 v[3], w[3];
	f32 length, sign, t1, invT1, t2;
	bool identity;

	// map first column to (*,0,0)
	length = Math::Sqrt(a[0][0] * a[0][0] + a[1][0] * a[1][0] +
		a[2][0] * a[2][0]);
	if (length > 0.0)
	{
		sign = (a[0][0] > 0.0f ? 1.0f : -1.0f);
		t1 = a[0][0] + sign * length;
		invT1 = 1.0f / t1;
		v[1] = a[1][0] * invT1;
		v[2] = a[2][0] * invT1;

		t2 = -2.0f / (1.0f + v[1] * v[1] + v[2] * v[2]);
		w[0] = t2 * (a[0][0] + a[1][0] * v[1] + a[2][0] * v[2]);
		w[1] = t2 * (a[0][1] + a[1][1] * v[1] + a[2][1] * v[2]);
		w[2] = t2 * (a[0][2] + a[1][2] * v[1] + a[2][2] * v[2]);
		a[0][0] += w[0];
		a[0][1] += w[1];
		a[0][2] += w[2];
		a[1][1] += v[1] * w[1];
		a[1][2] += v[1] * w[2];
		a[2][1] += v[2] * w[1];
		a[2][2] += v[2] * w[2];

		l[0][0] = 1.0f + t2;
		l[0][1] = l[1][0] = t2 * v[1];
		l[0][2] = l[2][0] = t2 * v[2];
		l[1][1] = 1.0f + t2 * v[1] * v[1];
		l[1][2] = l[2][1] = t2 * v[1] * v[2];
		l[2][2] = 1.0f + t2 * v[2] * v[2];
		identity = false;
	}
	else
	{
		l = IDENTITY;
		identity = true;
	}

	// map first row to (*,*,0)
	length = Math::Sqrt(a[0][1] * a[0][1] + a[0][2] * a[0][2]);
	if (length > 0.0)
	{
		sign = (a[0][1] > 0.0f ? 1.0f : -1.0f);
		t1 = a[0][1] + sign * length;
		v[2] = a[0][2] / t1;

		t2 = -2.0f / (1.0f + v[2] * v[2]);
		w[0] = t2 * (a[0][1] + a[0][2] * v[2]);
		w[1] = t2 * (a[1][1] + a[1][2] * v[2]);
		w[2] = t2 * (a[2][1] + a[2][2] * v[2]);
		a[0][1] += w[0];
		a[1][1] += w[1];
		a[1][2] += w[1] * v[2];
		a[2][1] += w[2];
		a[2][2] += w[2] * v[2];

		r[0][0] = 1.0;
		r[0][1] = r[1][0] = 0.0;
		r[0][2] = r[2][0] = 0.0;
		r[1][1] = 1.0f + t2;
		r[1][2] = r[2][1] = t2 * v[2];
		r[2][2] = 1.0f + t2 * v[2] * v[2];
	}
	else
	{
		r = IDENTITY;
	}

	// map second column to (*,*,0)
	length = Math::Sqrt(a[1][1] * a[1][1] + a[2][1] * a[2][1]);
	if (length > 0.0)
	{
		sign = (a[1][1] > 0.0f ? 1.0f : -1.0f);
		t1 = a[1][1] + sign * length;
		v[2] = a[2][1] / t1;

		t2 = -2.0f / (1.0f + v[2] * v[2]);
		w[1] = t2 * (a[1][1] + a[2][1] * v[2]);
		w[2] = t2 * (a[1][2] + a[2][2] * v[2]);
		a[1][1] += w[1];
		a[1][2] += w[2];
		a[2][2] += v[2] * w[2];

		f32 a = 1.0f + t2;
		f32 b = t2 * v[2];
		f32 c = 1.0f + b * v[2];

		if (identity)
		{
			l[0][0] = 1.0;
			l[0][1] = l[1][0] = 0.0;
			l[0][2] = l[2][0] = 0.0;
			l[1][1] = a;
			l[1][2] = l[2][1] = b;
			l[2][2] = c;
		}
		else
		{
			for (int row = 0; row < 3; row++)
			{
				f32 tmp0 = l[row][1];
				f32 tmp1 = l[row][2];
				l[row][1] = a * tmp0 + b * tmp1;
				l[row][2] = b * tmp0 + c * tmp1;
			}
		}
	}
}

void Matrix3::_golubKahanStep(Matrix3& a, Matrix3& l, Matrix3& r)
{
	f32 t11 = a[0][1] * a[0][1] + a[1][1] * a[1][1];
	f32 t22 = a[1][2] * a[1][2] + a[2][2] * a[2][2];
	f32 t12 = a[1][1] * a[1][2];
	f32 trace = t11 + t22;
	f32 diff = t11 - t22;
	f32 discr = Math::Sqrt(diff * diff + 4.0f * t12 * t12);
	f32 root1 = 0.5f * (trace + discr);
	f32 root2 = 0.5f * (trace - discr);

	// adjust right
	f32 y = a[0][0] - (Math::abs(root1 - t22) <=
						Math::abs(root2 - t22) ? root1 : root2);
	f32 z = a[0][1];
	f32 invLength = Math::InvSqrt(y * y + z * z);
	f32 sin = z * invLength;
	f32 cos = -y * invLength;

	f32 tmp0 = a[0][0];
	f32 tmp1 = a[0][1];
	a[0][0] = cos * tmp0 - sin * tmp1;
	a[0][1] = sin * tmp0 + cos * tmp1;
	a[1][0] = -sin * a[1][1];
	a[1][1] *= cos;

	size_t row;
	for (row = 0; row < 3; row++)
	{
		tmp0 = r[0][row];
		tmp1 = r[1][row];
		r[0][row] = cos * tmp0 - sin * tmp1;
		r[1][row] = sin * tmp0 + cos * tmp1;
	}

	// adjust left
	y = a[0][0];
	z = a[1][0];
	invLength = Math::InvSqrt(y * y + z * z);
	sin = z * invLength;
	cos = -y * invLength;

	a[0][0] = cos * a[0][0] - sin * a[1][0];
	tmp0 = a[0][1];
	tmp1 = a[1][1];
	a[0][1] = cos * tmp0 - sin * tmp1;
	a[1][1] = sin * tmp0 + cos * tmp1;
	a[0][2] = -sin * a[1][2];
	a[1][2] *= cos;

	size_t col;
	for (col = 0; col < 3; col++)
	{
		tmp0 = l[col][0];
		tmp1 = l[col][1];
		l[col][0] = cos * tmp0 - sin * tmp1;
		l[col][1] = sin * tmp0 + cos * tmp1;
	}

	// adjust right
	y = a[0][1];
	z = a[0][2];
	invLength = Math::InvSqrt(y * y + z * z);
	sin = z * invLength;
	cos = -y * invLength;

	a[0][1] = cos * a[0][1] - sin * a[0][2];
	tmp0 = a[1][1];
	tmp1 = a[1][2];
	a[1][1] = cos * tmp0 - sin * tmp1;
	a[1][2] = sin * tmp0 + cos * tmp1;
	a[2][1] = -sin * a[2][2];
	a[2][2] *= cos;

	for (row = 0; row < 3; row++)
	{
		tmp0 = r[1][row];
		tmp1 = r[2][row];
		r[1][row] = cos * tmp0 - sin * tmp1;
		r[2][row] = sin * tmp0 + cos * tmp1;
	}

	// adjust left
	y = a[1][1];
	z = a[2][1];
	invLength = Math::InvSqrt(y * y + z * z);
	sin = z * invLength;
	cos = -y * invLength;

	a[1][1] = cos * a[1][1] - sin * a[2][1];
	tmp0 = a[1][2];
	tmp1 = a[2][2];
	a[1][2] = cos * tmp0 - sin * tmp1;
	a[2][2] = sin * tmp0 + cos * tmp1;

	for (col = 0; col < 3; col++)
	{
		tmp0 = l[col][1];
		tmp1 = l[col][2];
		l[col][1] = cos * tmp0 - sin * tmp1;
		l[col][2] = sin * tmp0 + cos * tmp1;
	}
}

void Matrix3::singularValueDecomposition(Matrix3& kL, Vector3& kS, Matrix3& kR) const
{
	// temas: currently unused
	//const int iMax = 16;
	size_t iRow, iCol;

	Matrix3 kA = *this;
	_bidiagonalize(kA, kL, kR);

	for (unsigned int i = 0; i < _svdMaxIterations; i++)
	{
		f32 fTmp, fTmp0, fTmp1;
		f32 fSin0, fCos0, fTan0;
		f32 fSin1, fCos1, fTan1;

		bool bTest1 = (Math::abs(kA[0][1]) <=
			_svdEpsilon * (Math::abs(kA[0][0]) + Math::abs(kA[1][1])));
		bool bTest2 = (Math::abs(kA[1][2]) <=
			_svdEpsilon * (Math::abs(kA[1][1]) + Math::abs(kA[2][2])));
		if (bTest1)
		{
			if (bTest2)
			{
				kS[0] = kA[0][0];
				kS[1] = kA[1][1];
				kS[2] = kA[2][2];
				break;
			}

			// 2x2 closed form factorization
			fTmp = (kA[1][1] * kA[1][1] - kA[2][2] * kA[2][2] +
				kA[1][2] * kA[1][2]) / (kA[1][2] * kA[2][2]);
			fTan0 = 0.5f * (fTmp + Math::Sqrt(fTmp * fTmp + 4.0f));
			fCos0 = Math::InvSqrt(1.0f + fTan0 * fTan0);
			fSin0 = fTan0 * fCos0;

			for (iCol = 0; iCol < 3; iCol++)
			{
				fTmp0 = kL[iCol][1];
				fTmp1 = kL[iCol][2];
				kL[iCol][1] = fCos0 * fTmp0 - fSin0 * fTmp1;
				kL[iCol][2] = fSin0 * fTmp0 + fCos0 * fTmp1;
			}

			fTan1 = (kA[1][2] - kA[2][2] * fTan0) / kA[1][1];
			fCos1 = Math::InvSqrt(1.0f + fTan1 * fTan1);
			fSin1 = -fTan1 * fCos1;

			for (iRow = 0; iRow < 3; iRow++)
			{
				fTmp0 = kR[1][iRow];
				fTmp1 = kR[2][iRow];
				kR[1][iRow] = fCos1 * fTmp0 - fSin1 * fTmp1;
				kR[2][iRow] = fSin1 * fTmp0 + fCos1 * fTmp1;
			}

			kS[0] = kA[0][0];
			kS[1] = fCos0 * fCos1 * kA[1][1] -
				fSin1 * (fCos0 * kA[1][2] - fSin0 * kA[2][2]);
			kS[2] = fSin0 * fSin1 * kA[1][1] +
				fCos1 * (fSin0 * kA[1][2] + fCos0 * kA[2][2]);
			break;
		}
		if (bTest2)
		{
			// 2x2 closed form factorization
			fTmp = (kA[0][0] * kA[0][0] + kA[1][1] * kA[1][1] -
				kA[0][1] * kA[0][1]) / (kA[0][1] * kA[1][1]);
			fTan0 = 0.5f * (-fTmp + Math::Sqrt(fTmp * fTmp + 4.0f));
			fCos0 = Math::InvSqrt(1.0f + fTan0 * fTan0);
			fSin0 = fTan0 * fCos0;

			for (iCol = 0; iCol < 3; iCol++)
			{
				fTmp0 = kL[iCol][0];
				fTmp1 = kL[iCol][1];
				kL[iCol][0] = fCos0 * fTmp0 - fSin0 * fTmp1;
				kL[iCol][1] = fSin0 * fTmp0 + fCos0 * fTmp1;
			}

			fTan1 = (kA[0][1] - kA[1][1] * fTan0) / kA[0][0];
			fCos1 = Math::InvSqrt(1.0f + fTan1 * fTan1);
			fSin1 = -fTan1 * fCos1;

			for (iRow = 0; iRow < 3; iRow++)
			{
				fTmp0 = kR[0][iRow];
				fTmp1 = kR[1][iRow];
				kR[0][iRow] = fCos1 * fTmp0 - fSin1 * fTmp1;
				kR[1][iRow] = fSin1 * fTmp0 + fCos1 * fTmp1;
			}

			kS[0] = fCos0 * fCos1 * kA[0][0] -
				fSin1 * (fCos0 * kA[0][1] - fSin0 * kA[1][1]);
			kS[1] = fSin0 * fSin1 * kA[0][0] +
				fCos1 * (fSin0 * kA[0][1] + fCos0 * kA[1][1]);
			kS[2] = kA[2][2];
			break;
		}
		_golubKahanStep(kA, kL, kR);
	}

	// positize diagonal
	for (iRow = 0; iRow < 3; iRow++)
	{
		if (kS[iRow] < 0.0)
		{
			kS[iRow] = -kS[iRow];
			for (iCol = 0; iCol < 3; iCol++)
				kR[iRow][iCol] = -kR[iRow][iCol];
		}
	}
}

void Matrix3::singularValueComposition(const Matrix3& kL, const Vector3& kS, const Matrix3& kR)
{
	size_t iRow, iCol;
	Matrix3 kTmp;

	// product S*R
	for (iRow = 0; iRow < 3; iRow++)
	{
		for (iCol = 0; iCol < 3; iCol++)
			kTmp[iRow][iCol] = kS[iRow] * kR[iRow][iCol];
	}

	// product L*S*R
	for (iRow = 0; iRow < 3; iRow++)
	{
		for (iCol = 0; iCol < 3; iCol++)
		{
			m[iRow][iCol] = 0.0;
			for (int iMid = 0; iMid < 3; iMid++)
				m[iRow][iCol] += kL[iRow][iMid] * kTmp[iMid][iCol];
		}
	}
}

void Matrix3::orthonormalize()
{
	// Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
	// M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
	//
	//   q0 = m0/|m0|
	//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
	//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
	//
	// where |V| indicates length of vector V and A*B indicates dot
	// product of vectors A and B.

	// compute q0
	f32 fInvLength = Math::InvSqrt(m[0][0] * m[0][0]
		+ m[1][0] * m[1][0] +
		m[2][0] * m[2][0]);

	m[0][0] *= fInvLength;
	m[1][0] *= fInvLength;
	m[2][0] *= fInvLength;

	// compute q1
	f32 fDot0 =
		m[0][0] * m[0][1] +
		m[1][0] * m[1][1] +
		m[2][0] * m[2][1];

	m[0][1] -= fDot0 * m[0][0];
	m[1][1] -= fDot0 * m[1][0];
	m[2][1] -= fDot0 * m[2][0];

	fInvLength = Math::InvSqrt(m[0][1] * m[0][1] +
		m[1][1] * m[1][1] +
		m[2][1] * m[2][1]);

	m[0][1] *= fInvLength;
	m[1][1] *= fInvLength;
	m[2][1] *= fInvLength;

	// compute q2
	f32 fDot1 =
		m[0][1] * m[0][2] +
		m[1][1] * m[1][2] +
		m[2][1] * m[2][2];

	fDot0 =
		m[0][0] * m[0][2] +
		m[1][0] * m[1][2] +
		m[2][0] * m[2][2];

	m[0][2] -= fDot0 * m[0][0] + fDot1 * m[0][1];
	m[1][2] -= fDot0 * m[1][0] + fDot1 * m[1][1];
	m[2][2] -= fDot0 * m[2][0] + fDot1 * m[2][1];

	fInvLength = Math::InvSqrt(m[0][2] * m[0][2] +
		m[1][2] * m[1][2] +
		m[2][2] * m[2][2]);

	m[0][2] *= fInvLength;
	m[1][2] *= fInvLength;
	m[2][2] *= fInvLength;
}

void Matrix3::qduDecomposition(Matrix3& kQ, Vector3& kD, Vector3& kU) const
{
	// Factor M = QR = QDU where Q is orthogonal, D is diagonal,
	// and U is upper triangular with ones on its diagonal.  Algorithm uses
	// Gram-Schmidt orthogonalization (the QR algorithm).
	//
	// If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
	//
	//   q0 = m0/|m0|
	//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
	//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
	//
	// where |V| indicates length of vector V and A*B indicates dot
	// product of vectors A and B.  The matrix R has entries
	//
	//   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
	//   r10 = 0      r11 = q1*m1  r12 = q1*m2
	//   r20 = 0      r21 = 0      r22 = q2*m2
	//
	// so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
	// u02 = r02/r00, and u12 = r12/r11.

	// Q = rotation
	// D = scaling
	// U = shear

	// D stores the three diagonal entries r00, r11, r22
	// U stores the entries U[0] = u01, U[1] = u02, U[2] = u12

	// build orthogonal matrix Q
	f32 fInvLength = m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0];
	if (!Math::f32Equal(fInvLength, 0)) fInvLength = Math::InvSqrt(fInvLength);

	kQ[0][0] = m[0][0] * fInvLength;
	kQ[1][0] = m[1][0] * fInvLength;
	kQ[2][0] = m[2][0] * fInvLength;

	f32 fDot = kQ[0][0] * m[0][1] + kQ[1][0] * m[1][1] +
		kQ[2][0] * m[2][1];
	kQ[0][1] = m[0][1] - fDot * kQ[0][0];
	kQ[1][1] = m[1][1] - fDot * kQ[1][0];
	kQ[2][1] = m[2][1] - fDot * kQ[2][0];
	fInvLength = kQ[0][1] * kQ[0][1] + kQ[1][1] * kQ[1][1] + kQ[2][1] * kQ[2][1];
	if (!Math::f32Equal(fInvLength, 0)) fInvLength = Math::InvSqrt(fInvLength);

	kQ[0][1] *= fInvLength;
	kQ[1][1] *= fInvLength;
	kQ[2][1] *= fInvLength;

	fDot = kQ[0][0] * m[0][2] + kQ[1][0] * m[1][2] +
		kQ[2][0] * m[2][2];
	kQ[0][2] = m[0][2] - fDot * kQ[0][0];
	kQ[1][2] = m[1][2] - fDot * kQ[1][0];
	kQ[2][2] = m[2][2] - fDot * kQ[2][0];
	fDot = kQ[0][1] * m[0][2] + kQ[1][1] * m[1][2] +
		kQ[2][1] * m[2][2];
	kQ[0][2] -= fDot * kQ[0][1];
	kQ[1][2] -= fDot * kQ[1][1];
	kQ[2][2] -= fDot * kQ[2][1];
	fInvLength = kQ[0][2] * kQ[0][2] + kQ[1][2] * kQ[1][2] + kQ[2][2] * kQ[2][2];
	if (!Math::f32Equal(fInvLength, 0)) fInvLength = Math::InvSqrt(fInvLength);

	kQ[0][2] *= fInvLength;
	kQ[1][2] *= fInvLength;
	kQ[2][2] *= fInvLength;

	// guarantee that orthogonal matrix has determinant 1 (no reflections)
	f32 fDet = kQ[0][0] * kQ[1][1] * kQ[2][2] + kQ[0][1] * kQ[1][2] * kQ[2][0] +
		kQ[0][2] * kQ[1][0] * kQ[2][1] - kQ[0][2] * kQ[1][1] * kQ[2][0] -
		kQ[0][1] * kQ[1][0] * kQ[2][2] - kQ[0][0] * kQ[1][2] * kQ[2][1];

	if (fDet < 0.0)
	{
		for (size_t iRow = 0; iRow < 3; iRow++)
			for (size_t iCol = 0; iCol < 3; iCol++)
				kQ[iRow][iCol] = -kQ[iRow][iCol];
	}

	// build "right" matrix R
	Matrix3 kR;
	kR[0][0] = kQ[0][0] * m[0][0] + kQ[1][0] * m[1][0] +
		kQ[2][0] * m[2][0];
	kR[0][1] = kQ[0][0] * m[0][1] + kQ[1][0] * m[1][1] +
		kQ[2][0] * m[2][1];
	kR[1][1] = kQ[0][1] * m[0][1] + kQ[1][1] * m[1][1] +
		kQ[2][1] * m[2][1];
	kR[0][2] = kQ[0][0] * m[0][2] + kQ[1][0] * m[1][2] +
		kQ[2][0] * m[2][2];
	kR[1][2] = kQ[0][1] * m[0][2] + kQ[1][1] * m[1][2] +
		kQ[2][1] * m[2][2];
	kR[2][2] = kQ[0][2] * m[0][2] + kQ[1][2] * m[1][2] +
		kQ[2][2] * m[2][2];

	// the scaling component
	kD[0] = kR[0][0];
	kD[1] = kR[1][1];
	kD[2] = kR[2][2];

	// the shear component
	f32 fInvD0 = 1.0f / kD[0];
	kU[0] = kR[0][1] * fInvD0;
	kU[1] = kR[0][2] * fInvD0;
	kU[2] = kR[1][2] / kD[1];
}

f32 Matrix3::_maxCubicRoot(f32 afCoeff[3])
{
	// Spectral norm is for A^T*A, so characteristic polynomial
	// P(x) = c[0]+c[1]*x+c[2]*x^2+x^3 has three positive f32 roots.
	// This yields the assertions c[0] < 0 and c[2]*c[2] >= 3*c[1].

	// quick out for uniform scale (triple root)
	const f32 fOneThird = 1.0 / 3.0;
	const f32 fEpsilon = 1e-06;
	f32 fDiscr = afCoeff[2] * afCoeff[2] - 3.0f * afCoeff[1];
	if (fDiscr <= fEpsilon)
		return -fOneThird * afCoeff[2];

	// Compute an upper bound on roots of P(x).  This assumes that A^T*A
	// has been scaled by its largest entry.
	f32 fX = 1.0;
	f32 fPoly = afCoeff[0] + fX * (afCoeff[1] + fX * (afCoeff[2] + fX));
	if (fPoly < 0.0)
	{
		// uses a matrix norm to find an upper bound on maximum root
		fX = Math::abs(afCoeff[0]);
		f32 fTmp = 1.0f + Math::abs(afCoeff[1]);
		if (fTmp > fX)
			fX = fTmp;
		fTmp = 1.0f + Math::abs(afCoeff[2]);
		if (fTmp > fX)
			fX = fTmp;
	}

	// Newton's method to find root
	f32 fTwoC2 = 2.0f * afCoeff[2];
	for (int i = 0; i < 16; i++)
	{
		fPoly = afCoeff[0] + fX * (afCoeff[1] + fX * (afCoeff[2] + fX));
		if (Math::abs(fPoly) <= fEpsilon)
			return fX;

		f32 fDeriv = afCoeff[1] + fX * (fTwoC2 + 3.0f * fX);
		fX -= fPoly / fDeriv;
	}

	return fX;
}

f32 Matrix3::spectralNorm() const
{
	Matrix3 kP;
	size_t iRow, iCol;
	f32 fPmax = 0.0;
	for (iRow = 0; iRow < 3; iRow++)
	{
		for (iCol = 0; iCol < 3; iCol++)
		{
			kP[iRow][iCol] = 0.0;
			for (int iMid = 0; iMid < 3; iMid++)
			{
				kP[iRow][iCol] +=
					m[iMid][iRow] * m[iMid][iCol];
			}
			if (kP[iRow][iCol] > fPmax)
				fPmax = kP[iRow][iCol];
		}
	}

	f32 fInvPmax = 1.0f / fPmax;
	for (iRow = 0; iRow < 3; iRow++)
	{
		for (iCol = 0; iCol < 3; iCol++)
			kP[iRow][iCol] *= fInvPmax;
	}

	f32 afCoeff[3];
	afCoeff[0] = -(kP[0][0] * (kP[1][1] * kP[2][2] - kP[1][2] * kP[2][1]) +
		kP[0][1] * (kP[2][0] * kP[1][2] - kP[1][0] * kP[2][2]) +
		kP[0][2] * (kP[1][0] * kP[2][1] - kP[2][0] * kP[1][1]));
	afCoeff[1] = kP[0][0] * kP[1][1] - kP[0][1] * kP[1][0] +
		kP[0][0] * kP[2][2] - kP[0][2] * kP[2][0] +
		kP[1][1] * kP[2][2] - kP[1][2] * kP[2][1];
	afCoeff[2] = -(kP[0][0] + kP[1][1] + kP[2][2]);

	f32 fRoot = _maxCubicRoot(afCoeff);
	f32 fNorm = Math::Sqrt(fPmax * fRoot);
	return fNorm;
}

void Matrix3::toAngleAxis(Vector3& rkAxis, Radian& rfRadians) const
{
	// Let (x,y,z) be the unit-length axis and let A be an angle of rotation.
	// The rotation matrix is R = I + sin(A)*P + (1-cos(A))*P^2 where
	// I is the identity and
	//
	//       +-        -+
	//   P = |  0 -z +y |
	//       | +z  0 -x |
	//       | -y +x  0 |
	//       +-        -+
	//
	// If A > 0, R represents a counterclockwise rotation about the axis in
	// the sense of looking from the tip of the axis vector towards the
	// origin.  Some algebra will show that
	//
	//   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
	//
	// In the event that A = pi, R-R^t = 0 which prevents us from extracting
	// the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
	// P^2 = (R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
	// z^2-1.  We can solve these for axis (x,y,z).  Because the angle is pi,
	// it does not matter which sign you choose on the square roots.

	f32 fTrace = m[0][0] + m[1][1] + m[2][2];
	f32 fCos = 0.5f * (fTrace - 1.0f);
	rfRadians = Math::ACos(fCos); // in [0,PI]

	if (rfRadians > Radian(0.0))
	{
		if (rfRadians < Radian(Math::PI))
		{
			rkAxis.x = m[2][1] - m[1][2];
			rkAxis.y = m[0][2] - m[2][0];
			rkAxis.z = m[1][0] - m[0][1];
			rkAxis.normalize();
		}
		else
		{
			// angle is PI
			float fHalfInverse;
			if (m[0][0] >= m[1][1])
			{
				// r00 >= r11
				if (m[0][0] >= m[2][2])
				{
					// r00 is maximum diagonal term
					rkAxis.x = 0.5f * Math::Sqrt(m[0][0] -
						m[1][1] - m[2][2] + 1.0f);
					fHalfInverse = 0.5f / rkAxis.x;
					rkAxis.y = fHalfInverse * m[0][1];
					rkAxis.z = fHalfInverse * m[0][2];
				}
				else
				{
					// r22 is maximum diagonal term
					rkAxis.z = 0.5f * Math::Sqrt(m[2][2] -
						m[0][0] - m[1][1] + 1.0f);
					fHalfInverse = 0.5f / rkAxis.z;
					rkAxis.x = fHalfInverse * m[0][2];
					rkAxis.y = fHalfInverse * m[1][2];
				}
			}
			else
			{
				// r11 > r00
				if (m[1][1] >= m[2][2])
				{
					// r11 is maximum diagonal term
					rkAxis.y = 0.5f * Math::Sqrt(m[1][1] -
						m[0][0] - m[2][2] + 1.0f);
					fHalfInverse = 0.5f / rkAxis.y;
					rkAxis.x = fHalfInverse * m[0][1];
					rkAxis.z = fHalfInverse * m[1][2];
				}
				else
				{
					// r22 is maximum diagonal term
					rkAxis.z = 0.5f * Math::Sqrt(m[2][2] -
						m[0][0] - m[1][1] + 1.0f);
					fHalfInverse = 0.5f / rkAxis.z;
					rkAxis.x = fHalfInverse * m[0][2];
					rkAxis.y = fHalfInverse * m[1][2];
				}
			}
		}
	}
	else
	{
		// The angle is 0 and the matrix is the identity.  Any axis will
		// work, so just use the x-axis.
		rkAxis.x = 1.0;
		rkAxis.y = 0.0;
		rkAxis.z = 0.0;
	}
}

void Matrix3::fromAngleAxis(const Vector3& rkAxis, const Radian& fRadians)
{
	f32 fCos = Math::Cos(fRadians);
	f32 fSin = Math::Sin(fRadians);
	f32 fOneMinusCos = 1.0f - fCos;
	f32 fX2 = rkAxis.x * rkAxis.x;
	f32 fY2 = rkAxis.y * rkAxis.y;
	f32 fZ2 = rkAxis.z * rkAxis.z;
	f32 fXYM = rkAxis.x * rkAxis.y * fOneMinusCos;
	f32 fXZM = rkAxis.x * rkAxis.z * fOneMinusCos;
	f32 fYZM = rkAxis.y * rkAxis.z * fOneMinusCos;
	f32 fXSin = rkAxis.x * fSin;
	f32 fYSin = rkAxis.y * fSin;
	f32 fZSin = rkAxis.z * fSin;

	m[0][0] = fX2 * fOneMinusCos + fCos;
	m[0][1] = fXYM - fZSin;
	m[0][2] = fXZM + fYSin;
	m[1][0] = fXYM + fZSin;
	m[1][1] = fY2 * fOneMinusCos + fCos;
	m[1][2] = fYZM - fXSin;
	m[2][0] = fXZM - fYSin;
	m[2][1] = fYZM + fXSin;
	m[2][2] = fZ2 * fOneMinusCos + fCos;
}

bool Matrix3::toEulerAnglesXYZ(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const
{
	// rot =  cy*cz          -cy*sz           sy
	//        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
	//       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

	rfPAngle = Radian(Math::ASin(m[0][2]));
	if (rfPAngle < Radian(Math::HALF_PI))
	{
		if (rfPAngle > Radian(-Math::HALF_PI))
		{
			rfYAngle = Math::ATan2(-m[1][2], m[2][2]);
			rfRAngle = Math::ATan2(-m[0][1], m[0][0]);
			return true;
		}
		// WARNING.  Not a unique solution.
		Radian fRmY = Math::ATan2(m[1][0], m[1][1]);
		rfRAngle = Radian(0.0); // any angle works
		rfYAngle = rfRAngle - fRmY;
		return false;
	}
	// WARNING.  Not a unique solution.
	Radian fRpY = Math::ATan2(m[1][0], m[1][1]);
	rfRAngle = Radian(0.0); // any angle works
	rfYAngle = fRpY - rfRAngle;
	return false;
}

bool Matrix3::toEulerAnglesXZY(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const
{
	// rot =  cy*cz          -sz              cz*sy
	//        sx*sy+cx*cy*sz  cx*cz          -cy*sx+cx*sy*sz
	//       -cx*sy+cy*sx*sz  cz*sx           cx*cy+sx*sy*sz

	rfPAngle = Math::ASin(-m[0][1]);
	if (rfPAngle < Radian(Math::HALF_PI))
	{
		if (rfPAngle > Radian(-Math::HALF_PI))
		{
			rfYAngle = Math::ATan2(m[2][1], m[1][1]);
			rfRAngle = Math::ATan2(m[0][2], m[0][0]);
			return true;
		}
		// WARNING.  Not a unique solution.
		Radian fRmY = Math::ATan2(-m[2][0], m[2][2]);
		rfRAngle = Radian(0.0); // any angle works
		rfYAngle = rfRAngle - fRmY;
		return false;
	}
	// WARNING.  Not a unique solution.
	Radian fRpY = Math::ATan2(-m[2][0], m[2][2]);
	rfRAngle = Radian(0.0); // any angle works
	rfYAngle = fRpY - rfRAngle;
	return false;
}

bool Matrix3::toEulerAnglesYXZ(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const
{
	// rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
	//        cx*sz           cx*cz          -sx
	//       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

	rfPAngle = Math::ASin(-m[1][2]);
	if (rfPAngle < Radian(Math::HALF_PI))
	{
		if (rfPAngle > Radian(-Math::HALF_PI))
		{
			rfYAngle = Math::ATan2(m[0][2], m[2][2]);
			rfRAngle = Math::ATan2(m[1][0], m[1][1]);
			return true;
		}
		// WARNING.  Not a unique solution.
		Radian fRmY = Math::ATan2(-m[0][1], m[0][0]);
		rfRAngle = Radian(0.0); // any angle works
		rfYAngle = rfRAngle - fRmY;
		return false;
	}
	// WARNING.  Not a unique solution.
	Radian fRpY = Math::ATan2(-m[0][1], m[0][0]);
	rfRAngle = Radian(0.0); // any angle works
	rfYAngle = fRpY - rfRAngle;
	return false;
}

bool Matrix3::toEulerAnglesYZX(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const
{
	// rot =  cy*cz           sx*sy-cx*cy*sz  cx*sy+cy*sx*sz
	//        sz              cx*cz          -cz*sx
	//       -cz*sy           cy*sx+cx*sy*sz  cx*cy-sx*sy*sz

	rfPAngle = Math::ASin(m[1][0]);
	if (rfPAngle < Radian(Math::HALF_PI))
	{
		if (rfPAngle > Radian(-Math::HALF_PI))
		{
			rfYAngle = Math::ATan2(-m[2][0], m[0][0]);
			rfRAngle = Math::ATan2(-m[1][2], m[1][1]);
			return true;
		}
		// WARNING.  Not a unique solution.
		Radian fRmY = Math::ATan2(m[2][1], m[2][2]);
		rfRAngle = Radian(0.0); // any angle works
		rfYAngle = rfRAngle - fRmY;
		return false;
	}
	// WARNING.  Not a unique solution.
	Radian fRpY = Math::ATan2(m[2][1], m[2][2]);
	rfRAngle = Radian(0.0); // any angle works
	rfYAngle = fRpY - rfRAngle;
	return false;
}

bool Matrix3::toEulerAnglesZXY(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const
{
	// rot =  cy*cz-sx*sy*sz -cx*sz           cz*sy+cy*sx*sz
	//        cz*sx*sy+cy*sz  cx*cz          -cy*cz*sx+sy*sz
	//       -cx*sy           sx              cx*cy

	rfPAngle = Math::ASin(m[2][1]);
	if (rfPAngle < Radian(Math::HALF_PI))
	{
		if (rfPAngle > Radian(-Math::HALF_PI))
		{
			rfYAngle = Math::ATan2(-m[0][1], m[1][1]);
			rfRAngle = Math::ATan2(-m[2][0], m[2][2]);
			return true;
		}
		// WARNING.  Not a unique solution.
		Radian fRmY = Math::ATan2(m[0][2], m[0][0]);
		rfRAngle = Radian(0.0); // any angle works
		rfYAngle = rfRAngle - fRmY;
		return false;
	}
	// WARNING.  Not a unique solution.
	Radian fRpY = Math::ATan2(m[0][2], m[0][0]);
	rfRAngle = Radian(0.0); // any angle works
	rfYAngle = fRpY - rfRAngle;
	return false;
}

bool Matrix3::toEulerAnglesZYX(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const
{
	// rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
	//        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
	//       -sy              cy*sx           cx*cy

	rfPAngle = Math::ASin(-m[2][0]);
	if (rfPAngle < Radian(Math::HALF_PI))
	{
		if (rfPAngle > Radian(-Math::HALF_PI))
		{
			rfYAngle = Math::ATan2(m[1][0], m[0][0]);
			rfRAngle = Math::ATan2(m[2][1], m[2][2]);
			return true;
		}
		// WARNING.  Not a unique solution.
		Radian fRmY = Math::ATan2(-m[0][1], m[0][2]);
		rfRAngle = Radian(0.0); // any angle works
		rfYAngle = rfRAngle - fRmY;
		return false;
	}
	// WARNING.  Not a unique solution.
	Radian fRpY = Math::ATan2(-m[0][1], m[0][2]);
	rfRAngle = Radian(0.0); // any angle works
	rfYAngle = fRpY - rfRAngle;
	return false;
}

void Matrix3::fromEulerAnglesXYZ(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle)
{
	f32 fCos, fSin;

	fCos = Math::Cos(fYAngle);
	fSin = Math::Sin(fYAngle);
	Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

	fCos = Math::Cos(fPAngle);
	fSin = Math::Sin(fPAngle);
	Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

	fCos = Math::Cos(fRAngle);
	fSin = Math::Sin(fRAngle);
	Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

	*this = kXMat * (kYMat * kZMat);
}

void Matrix3::fromEulerAnglesXZY(const Radian& fYAngle, const Radian& fPAngle,
								const Radian& fRAngle)
{
	f32 fCos, fSin;

	fCos = Math::Cos(fYAngle);
	fSin = Math::Sin(fYAngle);
	Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

	fCos = Math::Cos(fPAngle);
	fSin = Math::Sin(fPAngle);
	Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

	fCos = Math::Cos(fRAngle);
	fSin = Math::Sin(fRAngle);
	Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

	*this = kXMat * (kZMat * kYMat);
}

void Matrix3::fromEulerAnglesYXZ(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle)
{
	f32 fCos, fSin;

	fCos = Math::Cos(fYAngle);
	fSin = Math::Sin(fYAngle);
	Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

	fCos = Math::Cos(fPAngle);
	fSin = Math::Sin(fPAngle);
	Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

	fCos = Math::Cos(fRAngle);
	fSin = Math::Sin(fRAngle);
	Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

	*this = kYMat * (kXMat * kZMat);
}

void Matrix3::fromEulerAnglesYZX(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle)
{
	f32 fCos, fSin;

	fCos = Math::Cos(fYAngle);
	fSin = Math::Sin(fYAngle);
	Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

	fCos = Math::Cos(fPAngle);
	fSin = Math::Sin(fPAngle);
	Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

	fCos = Math::Cos(fRAngle);
	fSin = Math::Sin(fRAngle);
	Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

	*this = kYMat * (kZMat * kXMat);
}

void Matrix3::fromEulerAnglesZXY(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle)
{
	f32 fCos, fSin;

	fCos = Math::Cos(fYAngle);
	fSin = Math::Sin(fYAngle);
	Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

	fCos = Math::Cos(fPAngle);
	fSin = Math::Sin(fPAngle);
	Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

	fCos = Math::Cos(fRAngle);
	fSin = Math::Sin(fRAngle);
	Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

	*this = kZMat * (kXMat * kYMat);
}

void Matrix3::fromEulerAnglesZYX(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle)
{
	f32 fCos, fSin;

	fCos = Math::Cos(fYAngle);
	fSin = Math::Sin(fYAngle);
	Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

	fCos = Math::Cos(fPAngle);
	fSin = Math::Sin(fPAngle);
	Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

	fCos = Math::Cos(fRAngle);
	fSin = Math::Sin(fRAngle);
	Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

	*this = kZMat * (kYMat * kXMat);
}

void Matrix3::tridiagonal(f32 afDiag[3], f32 afSubDiag[3])
{
	// Householder reduction T = Q^t M Q
	//   Input:
	//     mat, symmetric 3x3 matrix M
	//   Output:
	//     mat, orthogonal matrix Q
	//     diag, diagonal entries of T
	//     subd, subdiagonal entries of T (T is symmetric)

	f32 fA = m[0][0];
	f32 fB = m[0][1];
	f32 fC = m[0][2];
	f32 fD = m[1][1];
	f32 fE = m[1][2];
	f32 fF = m[2][2];

	afDiag[0] = fA;
	afSubDiag[2] = 0.0;
	if (Math::abs(fC) >= EPSILON)
	{
		f32 fLength = Math::Sqrt(fB * fB + fC * fC);
		f32 fInvLength = 1.0f / fLength;
		fB *= fInvLength;
		fC *= fInvLength;
		f32 fQ = 2.0f * fB * fE + fC * (fF - fD);
		afDiag[1] = fD + fC * fQ;
		afDiag[2] = fF - fC * fQ;
		afSubDiag[0] = fLength;
		afSubDiag[1] = fE - fB * fQ;
		m[0][0] = 1.0;
		m[0][1] = 0.0;
		m[0][2] = 0.0;
		m[1][0] = 0.0;
		m[1][1] = fB;
		m[1][2] = fC;
		m[2][0] = 0.0;
		m[2][1] = fC;
		m[2][2] = -fB;
	}
	else
	{
		afDiag[1] = fD;
		afDiag[2] = fF;
		afSubDiag[0] = fB;
		afSubDiag[1] = fE;
		m[0][0] = 1.0;
		m[0][1] = 0.0;
		m[0][2] = 0.0;
		m[1][0] = 0.0;
		m[1][1] = 1.0;
		m[1][2] = 0.0;
		m[2][0] = 0.0;
		m[2][1] = 0.0;
		m[2][2] = 1.0;
	}
}

bool Matrix3::qlAlgorithm(f32 afDiag[3], f32 afSubDiag[3])
{
	// QL iteration with implicit shifting to reduce matrix from tridiagonal
	// to diagonal

	for (int i0 = 0; i0 < 3; i0++)
	{
		const unsigned int iMaxIter = 32;
		unsigned int iIter;
		for (iIter = 0; iIter < iMaxIter; iIter++)
		{
			int i1;
			for (i1 = i0; i1 <= 1; i1++)
			{
				f32 fSum = Math::abs(afDiag[i1]) +
					Math::abs(afDiag[i1 + 1]);
				if (Math::abs(afSubDiag[i1]) + fSum == fSum)
					break;
			}
			if (i1 == i0)
				break;

			f32 fTmp0 = (afDiag[i0 + 1] - afDiag[i0]) / (2.0f * afSubDiag[i0]);
			f32 fTmp1 = Math::Sqrt(fTmp0 * fTmp0 + 1.0f);
			if (fTmp0 < 0.0)
				fTmp0 = afDiag[i1] - afDiag[i0] + afSubDiag[i0] / (fTmp0 - fTmp1);
			else
				fTmp0 = afDiag[i1] - afDiag[i0] + afSubDiag[i0] / (fTmp0 + fTmp1);
			f32 fSin = 1.0;
			f32 fCos = 1.0;
			f32 fTmp2 = 0.0;
			for (int i2 = i1 - 1; i2 >= i0; i2--)
			{
				f32 fTmp3 = fSin * afSubDiag[i2];
				f32 fTmp4 = fCos * afSubDiag[i2];
				if (Math::abs(fTmp3) >= Math::abs(fTmp0))
				{
					fCos = fTmp0 / fTmp3;
					fTmp1 = Math::Sqrt(fCos * fCos + 1.0f);
					afSubDiag[i2 + 1] = fTmp3 * fTmp1;
					fSin = 1.0f / fTmp1;
					fCos *= fSin;
				}
				else
				{
					fSin = fTmp3 / fTmp0;
					fTmp1 = Math::Sqrt(fSin * fSin + 1.0f);
					afSubDiag[i2 + 1] = fTmp0 * fTmp1;
					fCos = 1.0f / fTmp1;
					fSin *= fCos;
				}
				fTmp0 = afDiag[i2 + 1] - fTmp2;
				fTmp1 = (afDiag[i2] - fTmp0) * fSin + 2.0f * fTmp4 * fCos;
				fTmp2 = fSin * fTmp1;
				afDiag[i2 + 1] = fTmp0 + fTmp2;
				fTmp0 = fCos * fTmp1 - fTmp4;

				for (int iRow = 0; iRow < 3; iRow++)
				{
					fTmp3 = m[iRow][i2 + 1];
					m[iRow][i2 + 1] = fSin * m[iRow][i2] +
						fCos * fTmp3;
					m[iRow][i2] = fCos * m[iRow][i2] -
						fSin * fTmp3;
				}
			}
			afDiag[i0] -= fTmp2;
			afSubDiag[i0] = fTmp0;
			afSubDiag[i1] = 0.0;
		}

		if (iIter == iMaxIter)
		{
			// should not get here under normal circumstances
			return false;
		}
	}

	return true;
}

void Matrix3::eigenSolveSymmetric(f32 afEigenvalue[3], Vector3 akEigenvector[3]) const
{
	Matrix3 kMatrix = *this;
	f32 afSubDiag[3];
	kMatrix.tridiagonal(afEigenvalue, afSubDiag);
	kMatrix.qlAlgorithm(afEigenvalue, afSubDiag);

	for (size_t i = 0; i < 3; i++)
	{
		akEigenvector[i][0] = kMatrix[0][i];
		akEigenvector[i][1] = kMatrix[1][i];
		akEigenvector[i][2] = kMatrix[2][i];
	}

	// make eigenvectors form a right--handed system
	Vector3 kCross = akEigenvector[1].cross(akEigenvector[2]);
	f32 fDet = akEigenvector[0].dot(kCross);
	if (fDet < 0.0)
	{
		akEigenvector[2][0] = -akEigenvector[2][0];
		akEigenvector[2][1] = -akEigenvector[2][1];
		akEigenvector[2][2] = -akEigenvector[2][2];
	}
}

void Matrix3::tensorProduct(const Vector3& rkU, const Vector3& rkV, Matrix3& rkProduct)
{
	for (size_t iRow = 0; iRow < 3; iRow++)
	{
		for (size_t iCol = 0; iCol < 3; iCol++)
			rkProduct[iRow][iCol] = rkU[iRow] * rkV[iCol];
	}
}