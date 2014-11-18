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
	Vector3 prod;
	for (size_t row = 0; row < 3; row++)
	{
		prod[row] =
			m[row][0] * vector[0] +
			m[row][1] * vector[1] +
			m[row][2] * vector[2];
	}
	return prod;
}


namespace solo
{
	Vector3 operator*(const Vector3& vector, const Matrix3& matrix)
	{
		Vector3 result;
		for (size_t row = 0; row < 3; row++)
		{
			result[row] =
				vector[0] * matrix.m[0][row] +
				vector[1] * matrix.m[1][row] +
				vector[2] * matrix.m[2][row];
		}
		return result;
	}


	Matrix3 operator*(f32 fScalar, const Matrix3& rkMatrix)
	{
		Matrix3 prod;
		for (size_t row = 0; row < 3; row++)
		{
			for (size_t col = 0; col < 3; col++)
				prod[row][col] = fScalar * rkMatrix.m[row][col];
		}
		return prod;
	}
}


Matrix3 Matrix3::operator-() const
{
	Matrix3 result;
	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
			result[row][col] = -m[row][col];
	}
	return result;
}


Matrix3 Matrix3::operator*(f32 scalar) const
{
	Matrix3 result;
	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
			result[row][col] = scalar * m[row][col];
	}
	return result;
}


Matrix3 Matrix3::transpose() const
{
	Matrix3 result;
	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
			result[row][col] = m[col][row];
	}
	return result;
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
	length = Math::sqrt(a[0][0] * a[0][0] + a[1][0] * a[1][0] +
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
	length = Math::sqrt(a[0][1] * a[0][1] + a[0][2] * a[0][2]);
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
	length = Math::sqrt(a[1][1] * a[1][1] + a[2][1] * a[2][1]);
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
	f32 discr = Math::sqrt(diff * diff + 4.0f * t12 * t12);
	f32 root1 = 0.5f * (trace + discr);
	f32 root2 = 0.5f * (trace - discr);

	// adjust right
	f32 y = a[0][0] - (Math::abs(root1 - t22) <=
						Math::abs(root2 - t22) ? root1 : root2);
	f32 z = a[0][1];
	f32 invLength = Math::invSqrt(y * y + z * z);
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
	invLength = Math::invSqrt(y * y + z * z);
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
	invLength = Math::invSqrt(y * y + z * z);
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
	invLength = Math::invSqrt(y * y + z * z);
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
	size_t row, col;

	Matrix3 a = *this;
	_bidiagonalize(a, kL, kR);

	for (unsigned int i = 0; i < _svdMaxIterations; i++)
	{
		f32 tmp, tmp0, tmp1;
		f32 sin0, cos0, tan0;
		f32 sin1, cos1, tan1;

		bool test1 = (Math::abs(a[0][1]) <= _svdEpsilon * (Math::abs(a[0][0]) + Math::abs(a[1][1])));
		bool test2 = (Math::abs(a[1][2]) <= _svdEpsilon * (Math::abs(a[1][1]) + Math::abs(a[2][2])));
		if (test1)
		{
			if (test2)
			{
				kS[0] = a[0][0];
				kS[1] = a[1][1];
				kS[2] = a[2][2];
				break;
			}

			// 2x2 closed form factorization
			tmp = (a[1][1] * a[1][1] - a[2][2] * a[2][2] +
				a[1][2] * a[1][2]) / (a[1][2] * a[2][2]);
			tan0 = 0.5f * (tmp + Math::sqrt(tmp * tmp + 4.0f));
			cos0 = Math::invSqrt(1.0f + tan0 * tan0);
			sin0 = tan0 * cos0;

			for (col = 0; col < 3; col++)
			{
				tmp0 = kL[col][1];
				tmp1 = kL[col][2];
				kL[col][1] = cos0 * tmp0 - sin0 * tmp1;
				kL[col][2] = sin0 * tmp0 + cos0 * tmp1;
			}

			tan1 = (a[1][2] - a[2][2] * tan0) / a[1][1];
			cos1 = Math::invSqrt(1.0f + tan1 * tan1);
			sin1 = -tan1 * cos1;

			for (row = 0; row < 3; row++)
			{
				tmp0 = kR[1][row];
				tmp1 = kR[2][row];
				kR[1][row] = cos1 * tmp0 - sin1 * tmp1;
				kR[2][row] = sin1 * tmp0 + cos1 * tmp1;
			}

			kS[0] = a[0][0];
			kS[1] = cos0 * cos1 * a[1][1] - sin1 * (cos0 * a[1][2] - sin0 * a[2][2]);
			kS[2] = sin0 * sin1 * a[1][1] + cos1 * (sin0 * a[1][2] + cos0 * a[2][2]);
			break;
		}
		if (test2)
		{
			// 2x2 closed form factorization
			tmp = (a[0][0] * a[0][0] + a[1][1] * a[1][1] -
				a[0][1] * a[0][1]) / (a[0][1] * a[1][1]);
			tan0 = 0.5f * (-tmp + Math::sqrt(tmp * tmp + 4.0f));
			cos0 = Math::invSqrt(1.0f + tan0 * tan0);
			sin0 = tan0 * cos0;

			for (col = 0; col < 3; col++)
			{
				tmp0 = kL[col][0];
				tmp1 = kL[col][1];
				kL[col][0] = cos0 * tmp0 - sin0 * tmp1;
				kL[col][1] = sin0 * tmp0 + cos0 * tmp1;
			}

			tan1 = (a[0][1] - a[1][1] * tan0) / a[0][0];
			cos1 = Math::invSqrt(1.0f + tan1 * tan1);
			sin1 = -tan1 * cos1;

			for (row = 0; row < 3; row++)
			{
				tmp0 = kR[0][row];
				tmp1 = kR[1][row];
				kR[0][row] = cos1 * tmp0 - sin1 * tmp1;
				kR[1][row] = sin1 * tmp0 + cos1 * tmp1;
			}

			kS[0] = cos0 * cos1 * a[0][0] -
				sin1 * (cos0 * a[0][1] - sin0 * a[1][1]);
			kS[1] = sin0 * sin1 * a[0][0] +
				cos1 * (sin0 * a[0][1] + cos0 * a[1][1]);
			kS[2] = a[2][2];
			break;
		}
		_golubKahanStep(a, kL, kR);
	}

	// positize diagonal
	for (row = 0; row < 3; row++)
	{
		if (kS[row] < 0.0)
		{
			kS[row] = -kS[row];
			for (col = 0; col < 3; col++)
				kR[row][col] = -kR[row][col];
		}
	}
}

void Matrix3::singularValueComposition(const Matrix3& l, const Vector3& s, const Matrix3& r)
{
	size_t row, col;
	Matrix3 tmp;

	// product S*R
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
			tmp[row][col] = s[row] * r[row][col];
	}

	// product L*S*R
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			m[row][col] = 0.0;
			for (int iMid = 0; iMid < 3; iMid++)
				m[row][col] += l[row][iMid] * tmp[iMid][col];
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
	f32 invLength = Math::invSqrt(m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0]);

	m[0][0] *= invLength;
	m[1][0] *= invLength;
	m[2][0] *= invLength;

	// compute q1
	f32 dot0 =
		m[0][0] * m[0][1] +
		m[1][0] * m[1][1] +
		m[2][0] * m[2][1];

	m[0][1] -= dot0 * m[0][0];
	m[1][1] -= dot0 * m[1][0];
	m[2][1] -= dot0 * m[2][0];

	invLength = Math::invSqrt(m[0][1] * m[0][1] +
		m[1][1] * m[1][1] +
		m[2][1] * m[2][1]);

	m[0][1] *= invLength;
	m[1][1] *= invLength;
	m[2][1] *= invLength;

	// compute q2
	f32 fDot1 =
		m[0][1] * m[0][2] +
		m[1][1] * m[1][2] +
		m[2][1] * m[2][2];

	dot0 =
		m[0][0] * m[0][2] +
		m[1][0] * m[1][2] +
		m[2][0] * m[2][2];

	m[0][2] -= dot0 * m[0][0] + fDot1 * m[0][1];
	m[1][2] -= dot0 * m[1][0] + fDot1 * m[1][1];
	m[2][2] -= dot0 * m[2][0] + fDot1 * m[2][1];

	invLength = Math::invSqrt(m[0][2] * m[0][2] +
		m[1][2] * m[1][2] +
		m[2][2] * m[2][2]);

	m[0][2] *= invLength;
	m[1][2] *= invLength;
	m[2][2] *= invLength;
}

void Matrix3::qduDecomposition(Matrix3& q, Vector3& d, Vector3& u) const
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
	f32 invLength = m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0];
	if (!Math::f32Equal(invLength, 0)) invLength = Math::invSqrt(invLength);

	q[0][0] = m[0][0] * invLength;
	q[1][0] = m[1][0] * invLength;
	q[2][0] = m[2][0] * invLength;

	f32 dot = q[0][0] * m[0][1] + q[1][0] * m[1][1] + q[2][0] * m[2][1];
	q[0][1] = m[0][1] - dot * q[0][0];
	q[1][1] = m[1][1] - dot * q[1][0];
	q[2][1] = m[2][1] - dot * q[2][0];
	invLength = q[0][1] * q[0][1] + q[1][1] * q[1][1] + q[2][1] * q[2][1];
	if (!Math::f32Equal(invLength, 0))
		invLength = Math::invSqrt(invLength);

	q[0][1] *= invLength;
	q[1][1] *= invLength;
	q[2][1] *= invLength;

	dot = q[0][0] * m[0][2] + q[1][0] * m[1][2] + q[2][0] * m[2][2];
	q[0][2] = m[0][2] - dot * q[0][0];
	q[1][2] = m[1][2] - dot * q[1][0];
	q[2][2] = m[2][2] - dot * q[2][0];
	dot = q[0][1] * m[0][2] + q[1][1] * m[1][2] + q[2][1] * m[2][2];
	q[0][2] -= dot * q[0][1];
	q[1][2] -= dot * q[1][1];
	q[2][2] -= dot * q[2][1];
	invLength = q[0][2] * q[0][2] + q[1][2] * q[1][2] + q[2][2] * q[2][2];
	if (!Math::f32Equal(invLength, 0))
		invLength = Math::invSqrt(invLength);

	q[0][2] *= invLength;
	q[1][2] *= invLength;
	q[2][2] *= invLength;

	// guarantee that orthogonal matrix has determinant 1 (no reflections)
	f32 det = q[0][0] * q[1][1] * q[2][2] + q[0][1] * q[1][2] * q[2][0] +
		q[0][2] * q[1][0] * q[2][1] - q[0][2] * q[1][1] * q[2][0] -
		q[0][1] * q[1][0] * q[2][2] - q[0][0] * q[1][2] * q[2][1];

	if (det < 0.0)
	{
		for (size_t row = 0; row < 3; row++)
			for (size_t col = 0; col < 3; col++)
				q[row][col] = -q[row][col];
	}

	// build "right" matrix R
	Matrix3 r;
	r[0][0] = q[0][0] * m[0][0] + q[1][0] * m[1][0] +
		q[2][0] * m[2][0];
	r[0][1] = q[0][0] * m[0][1] + q[1][0] * m[1][1] +
		q[2][0] * m[2][1];
	r[1][1] = q[0][1] * m[0][1] + q[1][1] * m[1][1] +
		q[2][1] * m[2][1];
	r[0][2] = q[0][0] * m[0][2] + q[1][0] * m[1][2] +
		q[2][0] * m[2][2];
	r[1][2] = q[0][1] * m[0][2] + q[1][1] * m[1][2] +
		q[2][1] * m[2][2];
	r[2][2] = q[0][2] * m[0][2] + q[1][2] * m[1][2] +
		q[2][2] * m[2][2];

	// the scaling component
	d[0] = r[0][0];
	d[1] = r[1][1];
	d[2] = r[2][2];

	// the shear component
	f32 fInvD0 = 1.0f / d[0];
	u[0] = r[0][1] * fInvD0;
	u[1] = r[0][2] * fInvD0;
	u[2] = r[1][2] / d[1];
}

f32 Matrix3::_maxCubicRoot(f32 coeffs[3])
{
	// Spectral norm is for A^T*A, so characteristic polynomial
	// P(x) = c[0]+c[1]*x+c[2]*x^2+x^3 has three positive f32 roots.
	// This yields the assertions c[0] < 0 and c[2]*c[2] >= 3*c[1].

	// quick out for uniform scale (triple root)
	const f32 oneThird = 1.0 / 3.0;
	const f32 epsilon = 1e-06;
	f32 discr = coeffs[2] * coeffs[2] - 3.0f * coeffs[1];
	if (discr <= epsilon)
		return -oneThird * coeffs[2];

	// Compute an upper bound on roots of P(x).  This assumes that A^T*A
	// has been scaled by its largest entry.
	f32 x = 1.0;
	f32 poly = coeffs[0] + x * (coeffs[1] + x * (coeffs[2] + x));
	if ( poly < 0.0)
	{
		// uses a matrix norm to find an upper bound on maximum root
		x = Math::abs(coeffs[0]);
		f32 fTmp = 1.0f + Math::abs(coeffs[1]);
		if (fTmp > x)
			x = fTmp;
		fTmp = 1.0f + Math::abs(coeffs[2]);
		if (fTmp > x)
			x = fTmp;
	}

	// Newton's method to find root
	f32 fTwoC2 = 2.0f * coeffs[2];
	for (int i = 0; i < 16; i++)
	{
		poly = coeffs[0] + x * (coeffs[1] + x * (coeffs[2] + x));
		if (Math::abs( poly) <= epsilon)
			return x;

		f32 deriv = coeffs[1] + x * (fTwoC2 + 3.0f * x);
		x -=  poly / deriv;
	}

	return x;
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
	f32 fNorm = Math::sqrt(fPmax * fRoot);
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
	rfRadians = Math::acos(fCos); // in [0,PI]

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
					rkAxis.x = 0.5f * Math::sqrt(m[0][0] -
						m[1][1] - m[2][2] + 1.0f);
					fHalfInverse = 0.5f / rkAxis.x;
					rkAxis.y = fHalfInverse * m[0][1];
					rkAxis.z = fHalfInverse * m[0][2];
				}
				else
				{
					// r22 is maximum diagonal term
					rkAxis.z = 0.5f * Math::sqrt(m[2][2] -
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
					rkAxis.y = 0.5f * Math::sqrt(m[1][1] -
						m[0][0] - m[2][2] + 1.0f);
					fHalfInverse = 0.5f / rkAxis.y;
					rkAxis.x = fHalfInverse * m[0][1];
					rkAxis.z = fHalfInverse * m[1][2];
				}
				else
				{
					// r22 is maximum diagonal term
					rkAxis.z = 0.5f * Math::sqrt(m[2][2] -
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
	f32 fCos = Math::cos(fRadians);
	f32 fSin = Math::sin(fRadians);
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

	rfPAngle = Radian(Math::asin(m[0][2]));
	if (rfPAngle < Radian(Math::HALF_PI))
	{
		if (rfPAngle > Radian(-Math::HALF_PI))
		{
			rfYAngle = Math::atan2(-m[1][2], m[2][2]);
			rfRAngle = Math::atan2(-m[0][1], m[0][0]);
			return true;
		}
		// WARNING.  Not a unique solution.
		Radian fRmY = Math::atan2(m[1][0], m[1][1]);
		rfRAngle = Radian(0.0); // any angle works
		rfYAngle = rfRAngle - fRmY;
		return false;
	}
	// WARNING.  Not a unique solution.
	Radian fRpY = Math::atan2(m[1][0], m[1][1]);
	rfRAngle = Radian(0.0); // any angle works
	rfYAngle = fRpY - rfRAngle;
	return false;
}

bool Matrix3::toEulerAnglesXZY(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const
{
	// rot =  cy*cz          -sz              cz*sy
	//        sx*sy+cx*cy*sz  cx*cz          -cy*sx+cx*sy*sz
	//       -cx*sy+cy*sx*sz  cz*sx           cx*cy+sx*sy*sz

	rfPAngle = Math::asin(-m[0][1]);
	if (rfPAngle < Radian(Math::HALF_PI))
	{
		if (rfPAngle > Radian(-Math::HALF_PI))
		{
			rfYAngle = Math::atan2(m[2][1], m[1][1]);
			rfRAngle = Math::atan2(m[0][2], m[0][0]);
			return true;
		}
		// WARNING.  Not a unique solution.
		Radian fRmY = Math::atan2(-m[2][0], m[2][2]);
		rfRAngle = Radian(0.0); // any angle works
		rfYAngle = rfRAngle - fRmY;
		return false;
	}
	// WARNING.  Not a unique solution.
	Radian fRpY = Math::atan2(-m[2][0], m[2][2]);
	rfRAngle = Radian(0.0); // any angle works
	rfYAngle = fRpY - rfRAngle;
	return false;
}

bool Matrix3::toEulerAnglesYXZ(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const
{
	// rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
	//        cx*sz           cx*cz          -sx
	//       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

	rfPAngle = Math::asin(-m[1][2]);
	if (rfPAngle < Radian(Math::HALF_PI))
	{
		if (rfPAngle > Radian(-Math::HALF_PI))
		{
			rfYAngle = Math::atan2(m[0][2], m[2][2]);
			rfRAngle = Math::atan2(m[1][0], m[1][1]);
			return true;
		}
		// WARNING.  Not a unique solution.
		Radian fRmY = Math::atan2(-m[0][1], m[0][0]);
		rfRAngle = Radian(0.0); // any angle works
		rfYAngle = rfRAngle - fRmY;
		return false;
	}
	// WARNING.  Not a unique solution.
	Radian fRpY = Math::atan2(-m[0][1], m[0][0]);
	rfRAngle = Radian(0.0); // any angle works
	rfYAngle = fRpY - rfRAngle;
	return false;
}

bool Matrix3::toEulerAnglesYZX(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const
{
	// rot =  cy*cz           sx*sy-cx*cy*sz  cx*sy+cy*sx*sz
	//        sz              cx*cz          -cz*sx
	//       -cz*sy           cy*sx+cx*sy*sz  cx*cy-sx*sy*sz

	rfPAngle = Math::asin(m[1][0]);
	if (rfPAngle < Radian(Math::HALF_PI))
	{
		if (rfPAngle > Radian(-Math::HALF_PI))
		{
			rfYAngle = Math::atan2(-m[2][0], m[0][0]);
			rfRAngle = Math::atan2(-m[1][2], m[1][1]);
			return true;
		}
		// WARNING.  Not a unique solution.
		Radian fRmY = Math::atan2(m[2][1], m[2][2]);
		rfRAngle = Radian(0.0); // any angle works
		rfYAngle = rfRAngle - fRmY;
		return false;
	}
	// WARNING.  Not a unique solution.
	Radian fRpY = Math::atan2(m[2][1], m[2][2]);
	rfRAngle = Radian(0.0); // any angle works
	rfYAngle = fRpY - rfRAngle;
	return false;
}

bool Matrix3::toEulerAnglesZXY(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const
{
	// rot =  cy*cz-sx*sy*sz -cx*sz           cz*sy+cy*sx*sz
	//        cz*sx*sy+cy*sz  cx*cz          -cy*cz*sx+sy*sz
	//       -cx*sy           sx              cx*cy

	rfPAngle = Math::asin(m[2][1]);
	if (rfPAngle < Radian(Math::HALF_PI))
	{
		if (rfPAngle > Radian(-Math::HALF_PI))
		{
			rfYAngle = Math::atan2(-m[0][1], m[1][1]);
			rfRAngle = Math::atan2(-m[2][0], m[2][2]);
			return true;
		}
		// WARNING.  Not a unique solution.
		Radian fRmY = Math::atan2(m[0][2], m[0][0]);
		rfRAngle = Radian(0.0); // any angle works
		rfYAngle = rfRAngle - fRmY;
		return false;
	}
	// WARNING.  Not a unique solution.
	Radian fRpY = Math::atan2(m[0][2], m[0][0]);
	rfRAngle = Radian(0.0); // any angle works
	rfYAngle = fRpY - rfRAngle;
	return false;
}

bool Matrix3::toEulerAnglesZYX(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const
{
	// rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
	//        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
	//       -sy              cy*sx           cx*cy

	rfPAngle = Math::asin(-m[2][0]);
	if (rfPAngle < Radian(Math::HALF_PI))
	{
		if (rfPAngle > Radian(-Math::HALF_PI))
		{
			rfYAngle = Math::atan2(m[1][0], m[0][0]);
			rfRAngle = Math::atan2(m[2][1], m[2][2]);
			return true;
		}
		// WARNING.  Not a unique solution.
		Radian fRmY = Math::atan2(-m[0][1], m[0][2]);
		rfRAngle = Radian(0.0); // any angle works
		rfYAngle = rfRAngle - fRmY;
		return false;
	}
	// WARNING.  Not a unique solution.
	Radian fRpY = Math::atan2(-m[0][1], m[0][2]);
	rfRAngle = Radian(0.0); // any angle works
	rfYAngle = fRpY - rfRAngle;
	return false;
}

void Matrix3::fromEulerAnglesXYZ(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle)
{
	f32 fCos, fSin;

	fCos = Math::cos(fYAngle);
	fSin = Math::sin(fYAngle);
	Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

	fCos = Math::cos(fPAngle);
	fSin = Math::sin(fPAngle);
	Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

	fCos = Math::cos(fRAngle);
	fSin = Math::sin(fRAngle);
	Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

	*this = kXMat * (kYMat * kZMat);
}

void Matrix3::fromEulerAnglesXZY(const Radian& fYAngle, const Radian& fPAngle,
								const Radian& fRAngle)
{
	f32 fCos, fSin;

	fCos = Math::cos(fYAngle);
	fSin = Math::sin(fYAngle);
	Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

	fCos = Math::cos(fPAngle);
	fSin = Math::sin(fPAngle);
	Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

	fCos = Math::cos(fRAngle);
	fSin = Math::sin(fRAngle);
	Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

	*this = kXMat * (kZMat * kYMat);
}

void Matrix3::fromEulerAnglesYXZ(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle)
{
	f32 fCos, fSin;

	fCos = Math::cos(fYAngle);
	fSin = Math::sin(fYAngle);
	Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

	fCos = Math::cos(fPAngle);
	fSin = Math::sin(fPAngle);
	Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

	fCos = Math::cos(fRAngle);
	fSin = Math::sin(fRAngle);
	Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

	*this = kYMat * (kXMat * kZMat);
}

void Matrix3::fromEulerAnglesYZX(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle)
{
	f32 fCos, fSin;

	fCos = Math::cos(fYAngle);
	fSin = Math::sin(fYAngle);
	Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

	fCos = Math::cos(fPAngle);
	fSin = Math::sin(fPAngle);
	Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

	fCos = Math::cos(fRAngle);
	fSin = Math::sin(fRAngle);
	Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

	*this = kYMat * (kZMat * kXMat);
}

void Matrix3::fromEulerAnglesZXY(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle)
{
	f32 fCos, fSin;

	fCos = Math::cos(fYAngle);
	fSin = Math::sin(fYAngle);
	Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

	fCos = Math::cos(fPAngle);
	fSin = Math::sin(fPAngle);
	Matrix3 kXMat(1.0, 0.0, 0.0, 0.0, fCos, -fSin, 0.0, fSin, fCos);

	fCos = Math::cos(fRAngle);
	fSin = Math::sin(fRAngle);
	Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

	*this = kZMat * (kXMat * kYMat);
}

void Matrix3::fromEulerAnglesZYX(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle)
{
	f32 fCos, fSin;

	fCos = Math::cos(fYAngle);
	fSin = Math::sin(fYAngle);
	Matrix3 kZMat(fCos, -fSin, 0.0, fSin, fCos, 0.0, 0.0, 0.0, 1.0);

	fCos = Math::cos(fPAngle);
	fSin = Math::sin(fPAngle);
	Matrix3 kYMat(fCos, 0.0, fSin, 0.0, 1.0, 0.0, -fSin, 0.0, fCos);

	fCos = Math::cos(fRAngle);
	fSin = Math::sin(fRAngle);
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
		f32 fLength = Math::sqrt(fB * fB + fC * fC);
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
			f32 fTmp1 = Math::sqrt(fTmp0 * fTmp0 + 1.0f);
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
					fTmp1 = Math::sqrt(fCos * fCos + 1.0f);
					afSubDiag[i2 + 1] = fTmp3 * fTmp1;
					fSin = 1.0f / fTmp1;
					fCos *= fSin;
				}
				else
				{
					fSin = fTmp3 / fTmp0;
					fTmp1 = Math::sqrt(fSin * fSin + 1.0f);
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